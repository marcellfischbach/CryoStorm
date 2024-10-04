//
// Created by Marcell on 11.11.2023.
//

#include <ceOpenGL/gl4/pipeline/pointsm/gl4pointsmrenderer.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4pointlight.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceOpenGL/gl4/gl4rendertargetcube.hh>
#include <ceCore/csSettings.hh>
#include <ceCore/math/clipper/csCameraClipper.hh>
#include <ceCore/resource/csAssetManager.hh>
#include <ceCore/graphics/csCamera.hh>
#include <ceCore/graphics/csProjector.hh>
#include <ceCore/graphics/scene/iGfxScene.hh>
#include <ceCore/graphics/scene/csGfxMesh.hh>
#include <ceCore/graphics/shading/iShader.hh>
#include <ceCore/graphics/shading/iShaderAttribute.hh>


namespace cryo::opengl
{

GL4PointSMRenderer::GL4PointSMRenderer()
{
  for (size_t i = 0; i < m_pointLightShadowBuffer.size(); ++i)
  {
    m_pointLightShadowBuffer[i] = nullptr;
  }
}

void GL4PointSMRenderer::Initialize()
{
  const csSettingsFile &settings = csSettings::Get().Graphics();
  m_pointLightShadowBufferSize = settings.GetInt("point_light.shadow_map.size", 1024);
  std::string filter = settings.GetText("point_light.shadow_map.filter.mode", "Plain");
  if (filter == std::string("Plain"))
  {
    m_shadowSamplingMode = ShadowSamplingMode::Plain;
  }
  else if (filter == std::string("PCF"))
  {
    m_shadowSamplingMode = ShadowSamplingMode::PCF;
  }
  else if (filter == std::string("VSM"))
  {
    m_shadowSamplingMode = ShadowSamplingMode::VSM;
  }


  m_shadowMappingShader = csAssetManager::Get()->Get<iShader>(
      csResourceLocator("file://${engine}/opengl/gl4/pointsm/point_light_shadow_map.shader"));
  if (m_shadowMappingShader)
  {
    m_attrLightPosition = m_shadowMappingShader->GetShaderAttribute("LightPosition");
    m_attrMappingBias   = m_shadowMappingShader->GetShaderAttribute("MappingBias");
    m_attrShadowBuffer  = m_shadowMappingShader->GetShaderAttribute("ShadowBuffer");
    m_attrDepthBuffer   = m_shadowMappingShader->GetShaderAttribute("DepthBuffer");
  }


  csVector2f distance = settings.GetVector2f("point_light.shadow_map.filter.distance", csVector2f(1, 25));
  float      radius   = settings.GetFloat("point_light.shadow_map.filter.radius", 10.0f);
  float    samples        = settings.GetFloat("point_light.shadow_map.filter.samples", 25.0f);
  float    sampleDistance = settings.GetFloat("point_light.shadow_map.filter.sampleDistance", 0.1f);
  m_shadowMapFilter.Initialize(csVector2f(distance.x, distance.y - distance.x), radius, samples, sampleDistance);

}

void GL4PointSMRenderer::SetDevice(GL4Device *device)
{
  CS_SET(m_device, device);
}

void GL4PointSMRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  CS_SET(m_depthBuffer, depthBuffer);
  m_pointLightShadowMapWidth  = m_depthBuffer->GetWidth();
  m_pointLightShadowMapHeight = m_depthBuffer->GetHeight();
}

void GL4PointSMRenderer::SetScene(iGfxScene *scene)
{
  CS_SET(m_scene, scene);
}

void GL4PointSMRenderer::SetShadowMap(GL4RenderTarget2D *shadowMap)
{
  CS_SET(m_pointLightShadowMap, shadowMap);
}

GL4RenderTarget2D *GL4PointSMRenderer::GetShadowMap()
{
  return m_pointLightShadowMap;
}

void GL4PointSMRenderer::RenderShadow(const cryo::opengl::GL4PointLight *pointLight,
                                      const cryo::csCamera &camera,
                                      const cryo::csProjector &projector)
{
  RenderShadowBuffer(pointLight, camera, projector);
  RenderShadowMap(pointLight, camera, projector);
  FilterShadowMap();
}

void GL4PointSMRenderer::RenderShadowBuffer(const GL4PointLight *pointLight,
                                            const csCamera &camera,
                                            const csProjector &projector)
{

  float    near = pointLight->GetRange() * 0.001f;
  float      far  = pointLight->GetRange();
  csMatrix4f projection;
  csMatrix4f projectionInv;
  m_device->GetPerspectiveProjection(-near, near, -near, near, near, far, projection);
  m_device->GetPerspectiveProjectionInv(-near, near, -near, near, near, far, projectionInv);

  csVector3f pos = pointLight->GetPosition();
  csMatrix4f views[6];
  views[0].SetLookAt(pos, pos + csVector3f(1, 0, 0), csVector3f(0, -1, 0));
  views[1].SetLookAt(pos, pos + csVector3f(-1, 0, 0), csVector3f(0, -1, 0));
  views[2].SetLookAt(pos, pos + csVector3f(0, 1, 0), csVector3f(0, 0, -1));
  views[3].SetLookAt(pos, pos + csVector3f(0, -1, 0), csVector3f(0, 0, 1));
  views[4].SetLookAt(pos, pos + csVector3f(0, 0, -1), csVector3f(0, -1, 0));
  views[5].SetLookAt(pos, pos + csVector3f(0, 0, 1), csVector3f(0, -1, 0));

  csMatrix4f viewsInv[6];
  viewsInv[0].SetLookAtInv(pos, pos + csVector3f(1, 0, 0), csVector3f(0, -1, 0));
  viewsInv[1].SetLookAtInv(pos, pos + csVector3f(-1, 0, 0), csVector3f(0, -1, 0));
  viewsInv[2].SetLookAtInv(pos, pos + csVector3f(0, 1, 0), csVector3f(0, 0, -1));
  viewsInv[3].SetLookAtInv(pos, pos + csVector3f(0, -1, 0), csVector3f(0, 0, 1));
  viewsInv[4].SetLookAtInv(pos, pos + csVector3f(0, 0, -1), csVector3f(0, -1, 0));
  viewsInv[5].SetLookAtInv(pos, pos + csVector3f(0, 0, 1), csVector3f(0, -1, 0));

  m_device->SetProjectionMatrix(projection);

  for (size_t i = 0; i < 6; i++)
  {
    m_device->SetRenderTarget(GetShadowBuffer((eCubeFace)i));
    m_device->SetRenderBuffer(0);
    m_device->SetDepthWrite(true);
    m_device->SetDepthTest(true);
    m_device->SetBlending(false);
    m_device->SetColorWrite(true, true, true, true);
    m_device->Clear(true, csColor4f(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, true, 0);

    m_device->SetViewMatrix(views[i]);


    csCameraClipper clipper(viewsInv[i], projectionInv, false, true);

    size_t count = 0;
    m_scene->ScanMeshes(&clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static,
                        [this, &count](csGfxMesh *mesh)
                        {
                          mesh->RenderUnlit(m_device, eRP_Shadow);
                          count++;
                        }
    );
  }
}


void GL4PointSMRenderer::RenderShadowMap(const GL4PointLight *pointLight,
                                         const csCamera &camera,
                                         const csProjector &projector)
{
  GL4RenderTarget2D *target = GetShadowMapTemp();
  m_device->SetRenderTarget(target);
  m_device->SetRenderBuffer(0);
  m_device->SetDepthWrite(true);
  m_device->SetDepthTest(false);
  m_device->SetColorWrite(true, true, true, true);
  m_device->Clear(true, csColor4f(0.0, 0.0f, 0.0f, 0.0f), true, 1.0f, true, 0);

  m_device->SetShader(m_shadowMappingShader);
  m_device->ResetTextures();
  camera.Bind(m_device);
  projector.Bind(m_device);

  if (m_attrLightPosition)
  {
    m_attrLightPosition->Bind(pointLight->GetPosition());
  }
  if (m_attrMappingBias)
  {
    m_attrMappingBias->Bind(csVector3f(pointLight->GetRange() * 0.001f,
                                       pointLight->GetRange(),
                                       pointLight->GetShadowMapBias()));
  }

  if (m_attrShadowBuffer)
  {
    eTextureUnit unit = m_device->BindTexture(GetShadowBufferDepth());
    m_attrShadowBuffer->Bind(unit);
  }
  if (m_attrDepthBuffer)
  {
    eTextureUnit unit = m_device->BindTexture(m_depthBuffer);
    m_attrDepthBuffer->Bind(unit);
  }

  m_device->BindMatrices();
  m_device->BindStandardValues();

  m_device->RenderFullscreen();
}

void GL4PointSMRenderer::FilterShadowMap()
{
  m_device->ResetTextures();

  m_shadowMapFilter.Render(m_device,
                           m_depthBuffer,
                           GetShadowMapTemp()->GetColorTexture(0),
                           GetShadowMap()
  );

}

GL4RenderTarget2D *GL4PointSMRenderer::GetShadowBuffer(eCubeFace face)
{
  if (!m_pointLightShadowBuffer[face])
  {
    iRenderTarget2D::Descriptor desc {};
    desc.Width  = m_pointLightShadowBufferSize;
    desc.Height = m_pointLightShadowBufferSize;

    m_pointLightShadowBuffer[face] = QueryClass<GL4RenderTarget2D>(m_device->CreateRenderTarget(desc));


    if (m_shadowSamplingMode == ShadowSamplingMode::VSM || true)
    {
      m_pointLightShadowBuffer[face]->AddColorTexture(GetShadowBufferColor(), face);
    }
    m_pointLightShadowBuffer[face]->SetDepthTexture(GetShadowBufferDepth(), face);

    if (!m_pointLightShadowBuffer[face]->Compile())
    {
      printf("Unable to compile point light shadow buffer\n");
      m_pointLightShadowBuffer[face]->Release();
      m_pointLightShadowBuffer[face] = nullptr;
    }

  }
  return m_pointLightShadowBuffer[face];
}


iTextureCube *GL4PointSMRenderer::GetShadowBufferColor()
{
  if (!m_pointLightShadowBufferColor)
  {
    iTextureCube::Descriptor colorDesc {};
    colorDesc.Size    = m_pointLightShadowBufferSize;
    colorDesc.Format  = ePF_RGBA;
    colorDesc.MipMaps = false;

    m_pointLightShadowBufferColor = m_device->CreateTexture(colorDesc);
    m_pointLightShadowBufferColor->SetSampler(GetShadowBufferColorSampler());

  }
  return m_pointLightShadowBufferColor;
}

iTextureCube *GL4PointSMRenderer::GetShadowBufferDepth()
{
  if (!m_pointLightShadowBufferDepth)
  {
    iTextureCube::Descriptor colorDesc {};
    colorDesc.Size    = m_pointLightShadowBufferSize;
    colorDesc.Format  = ePF_Depth;
    colorDesc.MipMaps = false;

    m_pointLightShadowBufferDepth = m_device->CreateTexture(colorDesc);
    m_pointLightShadowBufferDepth->SetSampler(GetShadowBufferDepthSampler());

  }
  return m_pointLightShadowBufferDepth;
}

bool GL4PointSMRenderer::IsShadowMapValid(GL4RenderTarget2D *shadowMap) const
{
  return shadowMap
         && shadowMap->GetWidth() == m_pointLightShadowMapWidth
         && shadowMap->GetHeight() == m_pointLightShadowMapHeight;
}


GL4RenderTarget2D *GL4PointSMRenderer::GetShadowMapTemp()
{
  GL4RenderTarget2D *target = m_pointLightShadowMapTemp;
  if (target)
  {
    if (m_pointLightShadowMapWidth == target->GetWidth()
        && m_pointLightShadowMapHeight == target->GetHeight())
    {
      return target;
    }
    target->Release();
  }

  target                    = CreateShadowMap();
  m_pointLightShadowMapTemp = target;
  return target;
}


GL4RenderTarget2D *GL4PointSMRenderer::CreateShadowMap()
{
  iRenderTarget2D::Descriptor desc {};
  desc.Width  = (uint16_t) m_pointLightShadowMapWidth;
  desc.Height = (uint16_t) m_pointLightShadowMapHeight;
  auto target = QueryClass<GL4RenderTarget2D>(m_device->CreateRenderTarget(desc));

  iTexture2D::Descriptor colorDesc {};
  colorDesc.Width   = (uint16_t) m_pointLightShadowMapWidth;
  colorDesc.Height  = (uint16_t) m_pointLightShadowMapHeight;
  colorDesc.Format  = ePF_RGB;
  colorDesc.MipMaps = false;
  iTexture2D *colorTexture = m_device->CreateTexture(colorDesc);
  colorTexture->SetSampler(GetShadowMapColorSampler());
  target->AddColorTexture(colorTexture);
  target->SetDepthBuffer(ePF_Depth);

  if (!target->Compile())
  {
    target->Release();
    target = nullptr;
  }

  return target;
}

iSampler *GL4PointSMRenderer::GetShadowMapColorSampler()
{
  if (!m_shadowMapColorSampler)
  {
    m_shadowMapColorSampler = m_device->CreateSampler();
    m_shadowMapColorSampler->SetFilterMode(eFM_MinMagNearest);
    m_shadowMapColorSampler->SetAnisotropy(1);
    m_shadowMapColorSampler->SetAddressU(eTAM_Clamp);
    m_shadowMapColorSampler->SetAddressV(eTAM_Clamp);
    m_shadowMapColorSampler->SetAddressW(eTAM_Clamp);
  }
  return m_shadowMapColorSampler;
}


iSampler *GL4PointSMRenderer::GetShadowBufferColorSampler()
{
  if (!m_shadowBufferColorSampler)
  {
    m_shadowBufferColorSampler = m_device->CreateSampler();
    m_shadowBufferColorSampler->SetFilterMode(eFM_MinMagNearest);
    m_shadowBufferColorSampler->SetAnisotropy(1);
    m_shadowBufferColorSampler->SetAddressU(eTAM_Clamp);
    m_shadowBufferColorSampler->SetAddressV(eTAM_Clamp);
    m_shadowBufferColorSampler->SetAddressW(eTAM_Clamp);
  }
  return m_shadowBufferColorSampler;
}

iSampler *GL4PointSMRenderer::GetShadowBufferDepthSampler()
{
  if (!m_shadowMapDepthSampler)
  {
    m_shadowMapDepthSampler = m_device->CreateSampler();
    if (m_shadowSamplingMode == ShadowSamplingMode::PCF)
    {
      m_shadowMapDepthSampler->SetFilterMode(eFM_MinMagLinear);
    }
    else
    {
      m_shadowMapDepthSampler->SetFilterMode(eFM_MinMagNearest);
    }
    m_shadowMapDepthSampler->SetAnisotropy(1);
    m_shadowMapDepthSampler->SetAddressU(eTAM_Clamp);
    m_shadowMapDepthSampler->SetAddressV(eTAM_Clamp);
    m_shadowMapDepthSampler->SetAddressW(eTAM_Clamp);
    m_shadowMapDepthSampler->SetTextureCompareMode(eTCM_CompareToR);
    m_shadowMapDepthSampler->SetTextureCompareFunc(eCF_LessOrEqual);
    //m_shadowMapDepthSampler->SetTextureCompareMode(eTCM_None);
  }
  return m_shadowMapDepthSampler;
}


}