

#include <ceOpenGL/gl4/pipeline/forward/gl4forwardpointlightrenderer.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4pointlight.hh>
#include <ceOpenGL/gl4/gl4rendertargetcube.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceOpenGL/gl4/gl4texturecube.hh>

#include <ceCore/settings.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/isampler.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/math/clipper/cameraclipper.hh>
#include <ceCore/math/clipper/multiplaneclipper.hh>
#include <ceCore/math/clipper/sphereclipper.hh>
#include <ceCore/resource/assetmanager.hh>

#include <algorithm>
#include <GL/glew.h>



namespace ce::opengl
{

GL4ForwardPointLightRenderer::GL4ForwardPointLightRenderer()
{
  for (unsigned i = 0; i < MaxLights; i++)
  {
    m_pointLightShadowMap[i] = nullptr;
  }
}

GL4ForwardPointLightRenderer::~GL4ForwardPointLightRenderer()
{
  CE_RELEASE(m_shadowMapColorSampler);
}

void GL4ForwardPointLightRenderer::Initialize(Settings &settings)
{
  m_pointLightShadowBufferSize = settings.GetInt("point_light.shadow_map.size", 1024);
  std::string filter = settings.GetText("point_light.shadow_map.filter", "Plain");
  if (filter == std::string("Plain"))
  {
    m_shadowSamplingFilter = ShadowSamplingMode::Plain;
  }
  else if (filter == std::string("PCF"))
  {
    m_shadowSamplingFilter = ShadowSamplingMode::PCF;
  }
  else if (filter == std::string("VSM"))
  {
    m_shadowSamplingFilter = ShadowSamplingMode::VSM;
  }


  m_shadowMappingShader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/forward/point_light_shadow_map.shader"));
  if (m_shadowMappingShader)
  {
    m_attrLightPosition = m_shadowMappingShader->GetShaderAttribute("LightPosition");
    m_attrMappingBias = m_shadowMappingShader->GetShaderAttribute("MappingBias");
    m_attrShadowBuffer = m_shadowMappingShader->GetShaderAttribute("ShadowBuffer");
    m_attrDepthBuffer = m_shadowMappingShader->GetShaderAttribute("DepthBuffer");
  }


  Vector2f distance = settings.GetVector2f("point_light.shadow_map.filter.distance", Vector2f(1, 25));
  float radius = settings.GetFloat("point_light.shadow_map.filter.radius", 10.0f);
  float samples = settings.GetFloat("point_light.shadow_map.filter.samples", 25.0f);
  float sampleDistance = settings.GetFloat("point_light.shadow_map.filter.sampleDistance", 0.1f);
  m_shadowMapFilter.Initialize(Vector2f(distance.x, distance.y - distance.x), radius, samples, sampleDistance);

}

void GL4ForwardPointLightRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  m_depthBuffer = depthBuffer;
  m_shadowMapWidth = m_depthBuffer->GetWidth();
  m_shadowMapHeight = m_depthBuffer->GetHeight();
}




void GL4ForwardPointLightRenderer::Clear()
{
  m_shadowPointLights.clear();
}

void GL4ForwardPointLightRenderer::Add(GL4PointLight* pointLight)
{
  if (pointLight)
  {
    m_shadowPointLights.push_back(pointLight);
  }
}

void GL4ForwardPointLightRenderer::RenderShadowMaps(const Camera &camera,
                                                    const Projector &projector)
{
  size_t lightIdx = 0;
  SortLights();
  for (auto pointLight: m_shadowPointLights)
  {
    if (!m_device->MoreShadowMapsPossible())
    {
      break;
    }

    RenderShadow(pointLight, camera, projector, lightIdx++);
  }
}


void GL4ForwardPointLightRenderer::SortLights()
{

  std::sort(m_shadowPointLights.begin(), m_shadowPointLights.end(),
    [](GL4PointLight* light0, GL4PointLight* light1) {
      return light0->GetIntensity() > light1->GetIntensity();
    });
}


void GL4ForwardPointLightRenderer::RenderShadow(GL4PointLight *pointLight,
                                                      const Camera &camera,
                                                      const Projector &projector,
                                                      size_t lightIdx)
{
  RenderShadowBuffer(pointLight, camera, projector);
  RenderShadowMap(pointLight, camera, projector);
  FilterShadowMap(lightIdx);


  ApplyShadowMapToDevice(pointLight, lightIdx);
}


void GL4ForwardPointLightRenderer::RenderShadowBuffer(GL4PointLight *pointLight,
                                                      const Camera &camera,
                                                      const Projector &projector)
{
  if (m_shadowFBO == 0)
  {
    glGenFramebuffers(1, &m_shadowFBO);
    if (m_shadowFBO == 0)
    {
      return;
    }
  }





  float near = pointLight->GetRange() * 0.001f;
  float far = pointLight->GetRange();
  Matrix4f projection;
  Matrix4f projectionInv;
  m_device->GetPerspectiveProjection(-near, near, -near, near, near, far, projection);
  m_device->GetPerspectiveProjectionInv(-near, near, -near, near, near, far, projectionInv);

  Vector3f pos = pointLight->GetPosition();
  Matrix4f views[6];
  views[0].SetLookAt(pos, pos + Vector3f(1, 0, 0), Vector3f(0, -1, 0));
  views[1].SetLookAt(pos, pos + Vector3f(-1, 0, 0), Vector3f(0, -1, 0));
  views[2].SetLookAt(pos, pos + Vector3f(0, 1, 0), Vector3f(0, 0, -1));
  views[3].SetLookAt(pos, pos + Vector3f(0, -1, 0), Vector3f(0, 0, 1));
  views[4].SetLookAt(pos, pos + Vector3f(0, 0, -1), Vector3f(0, -1, 0));
  views[5].SetLookAt(pos, pos + Vector3f(0, 0, 1), Vector3f(0, -1, 0));

  Matrix4f viewsInv[6];
  viewsInv[0].SetLookAtInv(pos, pos + Vector3f(1, 0, 0), Vector3f(0, -1, 0));
  viewsInv[1].SetLookAtInv(pos, pos + Vector3f(-1, 0, 0), Vector3f(0, -1, 0));
  viewsInv[2].SetLookAtInv(pos, pos + Vector3f(0, 1, 0), Vector3f(0, 0, -1));
  viewsInv[3].SetLookAtInv(pos, pos + Vector3f(0, -1, 0), Vector3f(0, 0, 1));
  viewsInv[4].SetLookAtInv(pos, pos + Vector3f(0, 0, -1), Vector3f(0, -1, 0));
  viewsInv[5].SetLookAtInv(pos, pos + Vector3f(0, 0, 1), Vector3f(0, -1, 0));

  GL4TextureCube* colorTexture = GetPointLightShadowBufferColor ();
  GL4TextureCube* depthTexture = GetPointLightShadowBufferDepth ();


  m_device->SetProjectionMatrix(projection);

  glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
  for (int i=0; i<6; i++)
  {

    if (colorTexture)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             colorTexture->GetName(),
                             0);
    }
    if (depthTexture)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER,
          //GL_DEPTH_STENCIL_ATTACHMENT,
                             GL_DEPTH_ATTACHMENT,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             depthTexture->GetName(),
                             0);
    }
    m_device->SetColorWrite(false, false, false, false);

    m_device->SetViewport(0, 0, (uint16_t)m_pointLightShadowBufferSize, (uint16_t)m_pointLightShadowBufferSize);
    m_device->Clear(false, Color4f(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, false, 0);

    m_device->SetViewMatrix(views[i]);


    CameraClipper clipper (viewsInv[i], projectionInv, false, true);

    m_scene->ScanMeshes(&clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static,
                        [this](GfxMesh *mesh) {
                          mesh->RenderUnlit(m_device, eRP_Shadow);
                        }
    );
    m_device->SetColorWrite(true, true, true, true);
  }
  m_device->SetRenderTarget(nullptr);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void GL4ForwardPointLightRenderer::RenderShadowMap(GL4PointLight *pointLight,
                                                   const Camera &camera,
                                                   const Projector &projector)
{
  m_device->ResetTextures();
  GL4RenderTarget2D *target = GetPointLightShadowMapTemp();
  m_device->SetRenderTarget(target);
  m_device->SetDepthWrite(true);
  m_device->SetDepthTest(false);
  m_device->SetColorWrite(true, true, true, true);
  m_device->Clear(true, Color4f(0.0, 0.0f, 0.0f, 0.0f), true, 1.0f, true, 0);

  m_device->SetShader(m_shadowMappingShader);
  camera.Bind(m_device);
  projector.Bind(m_device);

  if (m_attrLightPosition)
  {
      m_attrLightPosition->Bind(pointLight->GetPosition());
  }
  if (m_attrMappingBias)
  {
    m_attrMappingBias->Bind(Vector3f(pointLight->GetRange() * 0.001f, pointLight->GetRange(), pointLight->GetShadowMapBias()));
  }

  if (m_attrShadowBuffer)
  {
    eTextureUnit unit = m_device->BindTexture(GetPointLightShadowBufferDepth());
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

void GL4ForwardPointLightRenderer::FilterShadowMap(size_t lightIdx)
{
    m_device->ResetTextures();
    GL4RenderTarget2D* target = GetPointLightShadowMap(lightIdx);

    m_shadowMapFilter.Render(m_device,
        m_depthBuffer,
        GetPointLightShadowMapTemp()->GetColorTexture(0),
        target
    );

}

void GL4ForwardPointLightRenderer::ApplyShadowMapToDevice(GL4PointLight *pointLight, size_t lightIdx)
{
    if (lightIdx >= MaxLights)
    {
        return;
    }

    GL4RenderTarget2D* target = m_pointLightShadowMap[lightIdx];
    if (!target)
    {
        return;
    }

    m_device->SetLightShadowMap(pointLight, target->GetColorTexture(0));
}


GL4TextureCube *GL4ForwardPointLightRenderer::GetPointLightShadowBufferColor()
{
  if (!m_shadowBufferColor)
  {
    iTextureCube::Descriptor desc = {};
    desc.Format = ePF_RGB;
    desc.MipMaps = false;
    desc.Size = (uint16_t)m_pointLightShadowBufferSize;
    iTextureCube *color = m_device->CreateTexture(desc);
    color->SetSampler(GetShadowBufferColorSampler());
    m_shadowBufferColor = QueryClass<GL4TextureCube>(color);
  }
  return m_shadowBufferColor;
}

GL4TextureCube *GL4ForwardPointLightRenderer::GetPointLightShadowBufferDepth()
{
    if (!m_shadowBufferDepth)
    {
      iTextureCube::Descriptor desc = {};
      desc.Format = ePF_Depth;
      desc.MipMaps = false;
      desc.Size = (uint16_t)m_pointLightShadowBufferSize;
      iTextureCube *depth = m_device->CreateTexture(desc);
      depth->SetSampler(GetShadowBufferDepthSampler());
      m_shadowBufferDepth = QueryClass<GL4TextureCube>(depth);
    }
    return m_shadowBufferDepth;
}


iSampler *GL4ForwardPointLightRenderer::GetShadowBufferColorSampler()
{
  if (!m_shadowBufferColorSampler)
  {
    m_shadowBufferColorSampler = m_device->CreateSampler();
    m_shadowBufferColorSampler->SetAddressU(eTAM_Clamp);
    m_shadowBufferColorSampler->SetAddressV(eTAM_Clamp);
    m_shadowBufferColorSampler->SetAddressW(eTAM_Clamp);
    m_shadowBufferColorSampler->SetFilterMode(eFM_MinMagLinear);
    m_shadowBufferColorSampler->SetAnisotropy(1);
  }
  return m_shadowBufferColorSampler;
}

iSampler *GL4ForwardPointLightRenderer::GetShadowBufferDepthSampler()
{
    if (!m_shadowBufferDepthSampler)
    {
        m_shadowBufferDepthSampler = m_device->CreateSampler();
        if (m_shadowSamplingFilter== ShadowSamplingMode::PCF)
        {
            m_shadowBufferDepthSampler->SetFilterMode(eFM_MinMagLinear);
        }
        else
        {
            m_shadowBufferDepthSampler->SetFilterMode(eFM_MinMagNearest);
        }
        m_shadowBufferDepthSampler->SetAnisotropy(1);
        m_shadowBufferDepthSampler->SetAddressU(eTAM_Clamp);
        m_shadowBufferDepthSampler->SetAddressV(eTAM_Clamp);
        m_shadowBufferDepthSampler->SetAddressW(eTAM_Clamp);
        m_shadowBufferDepthSampler->SetTextureCompareMode(eTCM_CompareToR);
        m_shadowBufferDepthSampler->SetTextureCompareFunc(eCF_LessOrEqual);
    }
    return m_shadowBufferDepthSampler;
}


GL4RenderTarget2D* GL4ForwardPointLightRenderer::CreatePointLightShadowMap()
{
  iRenderTarget2D::Descriptor desc{};
  desc.Width = (uint16_t) m_shadowMapWidth;
  desc.Height = (uint16_t) m_shadowMapHeight;
  auto target = QueryClass<GL4RenderTarget2D>(m_device->CreateRenderTarget(desc));

  iTexture2D::Descriptor colorDesc{};
  colorDesc.Width = (uint16_t) m_shadowMapWidth;
  colorDesc.Height = (uint16_t) m_shadowMapHeight;
  colorDesc.Format = ePF_RGB;
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

GL4RenderTarget2D* GL4ForwardPointLightRenderer::GetPointLightShadowMapTemp()
{
  GL4RenderTarget2D *target = m_pointLightShadowMapTemp;
  if (target)
  {
    if (m_shadowMapWidth == target->GetWidth()
        && m_shadowMapHeight == target->GetHeight())
    {
      return target;
    }
    target->Release();
  }

  target = CreatePointLightShadowMap();
  m_pointLightShadowMapTemp = target;
  return target;

}

GL4RenderTarget2D* GL4ForwardPointLightRenderer::GetPointLightShadowMap(Size lightIdx)
{
  if (lightIdx >= MaxLights)
  {
    return nullptr;
  }

  GL4RenderTarget2D *target = m_pointLightShadowMap[lightIdx];
  if (target)
  {
    if (m_shadowMapWidth == target->GetWidth()
        && m_shadowMapHeight == target->GetHeight())
    {
      return target;
    }
    target->Release();
  }

  target = CreatePointLightShadowMap();
  m_pointLightShadowMap[lightIdx] = target;
  return target;
}

iSampler* GL4ForwardPointLightRenderer::GetShadowMapColorSampler()
{
  if (!m_shadowMapColorSampler)
  {
    m_shadowMapColorSampler = m_device->CreateSampler();
    m_shadowMapColorSampler->SetAddressU(eTAM_Clamp);
    m_shadowMapColorSampler->SetAddressV(eTAM_Clamp);
    m_shadowMapColorSampler->SetAddressW(eTAM_Clamp);
    m_shadowMapColorSampler->SetFilterMode(eFM_MinMagLinear);
    m_shadowMapColorSampler->SetAnisotropy(1);
  }
  return m_shadowMapColorSampler;
}






#if 0

GL4RenderTargetCube* GL4ForwardPointLightRenderer::GetPointLightShadowMap(Size idx)
{


  if (m_pointLightShadowMap.size() <= idx)
  {
    iRenderTargetCube::Descriptor desc{};
    desc.Size = static_cast<uint16_t>(m_pointLightShadowBufferSize);
    GL4RenderTargetCube* cubeMap = static_cast<GL4RenderTargetCube*>(m_device->CreateRenderTarget(desc));


    if (m_shadowSamplingFilter == ShadowMapFilter::VSM)
    {
      iTextureCube::Descriptor colorDesc{};
      colorDesc.Size = desc.Size;
      colorDesc.Format = ePF_RGBA;
      colorDesc.MipMaps = false;
      iTextureCube* colorTexture = m_device->CreateTexture(colorDesc);
      colorTexture->SetSampler(GetShadowMapColorSampler());
      cubeMap->AddColorTexture(colorTexture);
      colorTexture->Release();
    }

    iTextureCube::Descriptor depthDesc{};
    depthDesc.Size = desc.Size;
    depthDesc.Format = ePF_Depth;
    depthDesc.MipMaps = false;
    iTextureCube* depthTexture = m_device->CreateTexture(depthDesc);
    depthTexture->SetSampler(GetShadowMapDepthSampler());
    cubeMap->SetDepthTexture(depthTexture);
    depthTexture->Release();

    if (!cubeMap->Compile())
    {
      cubeMap->Release();
      cubeMap = nullptr;
      return nullptr;
    }
    else
    {
      m_pointLightShadowMap.push_back(cubeMap);
    }
  }
  return m_pointLightShadowMap[idx];
}

iSampler *GL4ForwardPointLightRenderer::GetShadowMapColorSampler()
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


iSampler* GL4ForwardPointLightRenderer::GetShadowMapDepthSampler()
{
  if (!m_shadowMapDepthSampler)
  {
    m_shadowMapDepthSampler = m_device->CreateSampler();
    if (m_shadowSamplingFilter == ShadowMapFilter::PCF)
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
  }
  return m_shadowMapDepthSampler;
}



void GL4ForwardPointLightRenderer::RenderPointShadowMaps(GL4PointLight* pointLight, GL4RenderTargetCube* shadowMap)
{
  m_device->SetRenderTarget(shadowMap);
  m_device->SetViewport(0, 0, shadowMap->GetSize(), shadowMap->GetSize());
  m_device->Clear(true, Color4f(0.0f, 0.0f, 0.5f, 1.0f), true, 1.0f, false, 0);


  float near = 0.1f;
  float far = pointLight->GetRange();
  Matrix4f projection;
  m_device->GetPerspectiveProjection(-near, near, -near, near, near, far, projection);
  Matrix4f projections[] = {
    projection,
    projection,
    projection,
    projection,
    projection,
    projection
  };
  Vector3f pos = pointLight->GetPosition();
  Matrix4f views[6];
  views[0].SetLookAt(pos, pos + Vector3f(1, 0, 0), Vector3f(0, -1, 0));
  views[1].SetLookAt(pos, pos + Vector3f(-1, 0, 0), Vector3f(0, -1, 0));
  views[2].SetLookAt(pos, pos + Vector3f(0, 1, 0), Vector3f(0, 0, -1));
  views[3].SetLookAt(pos, pos + Vector3f(0, -1, 0), Vector3f(0, 0, 1));
  views[4].SetLookAt(pos, pos + Vector3f(0, 0, -1), Vector3f(0, -1, 0));
  views[5].SetLookAt(pos, pos + Vector3f(0, 0, 1), Vector3f(0, -1, 0));


  m_device->SetShadowMapProjectionMatrices(projections, 6);
  m_device->SetShadowMapViewMatrices(views, 6);

  SphereClipper clipper(pos, pointLight->GetRange());


  glColorMask(false, false, false, false);
  m_scene->ScanMeshes(&clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static,
                      [this](GfxMesh* mesh)
    {
      mesh->RenderUnlit(m_device, eRP_ShadowCube);
    }
  );

  glColorMask(true, true, true, true);

}

GLuint fbo_shadow = 0;

void GL4ForwardPointLightRenderer::RenderPointShadowMapsStraight(GL4PointLight *pointLight, GL4RenderTargetCube *shadowMap)
{

  if (fbo_shadow == 0)
  {
    glGenFramebuffers(1, &fbo_shadow);
    if (fbo_shadow == 0)
    {
      return;
    }
  }

  

  

  float near = 0.1f;
  float far = pointLight->GetRange();
  Matrix4f projection;
  Matrix4f projectionInv;
  m_device->GetPerspectiveProjection(-near, near, -near, near, near, far, projection);
  m_device->GetPerspectiveProjectionInv(-near, near, -near, near, near, far, projectionInv);
  Matrix4f projections[] = {
      projection,
      projection,
      projection,
      projection,
      projection,
      projection
  };
  Vector3f pos = pointLight->GetPosition();
  Matrix4f views[6];
  views[0].SetLookAt(pos, pos + Vector3f(1, 0, 0), Vector3f(0, -1, 0));
  views[1].SetLookAt(pos, pos + Vector3f(-1, 0, 0), Vector3f(0, -1, 0));
  views[2].SetLookAt(pos, pos + Vector3f(0, 1, 0), Vector3f(0, 0, -1));
  views[3].SetLookAt(pos, pos + Vector3f(0, -1, 0), Vector3f(0, 0, 1));
  views[4].SetLookAt(pos, pos + Vector3f(0, 0, -1), Vector3f(0, -1, 0));
  views[5].SetLookAt(pos, pos + Vector3f(0, 0, 1), Vector3f(0, -1, 0));

  Matrix4f viewsInv[6];
  viewsInv[0].SetLookAtInv(pos, pos + Vector3f(1, 0, 0), Vector3f(0, -1, 0));
  viewsInv[1].SetLookAtInv(pos, pos + Vector3f(-1, 0, 0), Vector3f(0, -1, 0));
  viewsInv[2].SetLookAtInv(pos, pos + Vector3f(0, 1, 0), Vector3f(0, 0, -1));
  viewsInv[3].SetLookAtInv(pos, pos + Vector3f(0, -1, 0), Vector3f(0, 0, 1));
  viewsInv[4].SetLookAtInv(pos, pos + Vector3f(0, 0, -1), Vector3f(0, -1, 0));
  viewsInv[5].SetLookAtInv(pos, pos + Vector3f(0, 0, 1), Vector3f(0, -1, 0));

  iTextureCube* cubeColorTexture = shadowMap->GetColorTexture(0);
  iTextureCube* cubeDepthTexture = shadowMap->GetDepthTexture();

  GL4TextureCube* colorTexture = cubeColorTexture ? cubeColorTexture->Query<GL4TextureCube>() : nullptr;
  GL4TextureCube* depthTexture = cubeDepthTexture ? cubeDepthTexture->Query<GL4TextureCube>() : nullptr;


  glBindFramebuffer(GL_FRAMEBUFFER, fbo_shadow);
  for (int i=0; i<6; i++)
  {
    
    if (colorTexture)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        colorTexture->GetName(),
        0);
    }
    if (depthTexture)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER,
        //GL_DEPTH_STENCIL_ATTACHMENT,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        depthTexture->GetName(),
        0);
    }
    glColorMask(false, false, false, false);

    m_device->SetViewport(0, 0, shadowMap->GetSize(), shadowMap->GetSize());
    m_device->Clear(false, Color4f(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, false, 0);

    m_device->SetProjectionMatrix(projections[i]);
    m_device->SetViewMatrix(views[i]);


    CameraClipper clppr (viewsInv[i], projectionInv, false, true);

    m_scene->ScanMeshes(&clppr, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static,
                        [this](GfxMesh *mesh) {
                          mesh->RenderUnlit(m_device, eRP_Shadow);
                        }
    );
    glColorMask(true, true, true, true);
  }
  m_device->SetRenderTarget(nullptr);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
#endif


void GL4ForwardPointLightRenderer::SetDevice(iDevice* device)
{
  m_device = QueryClass<GL4Device>(device);
}

void GL4ForwardPointLightRenderer::SetScene(iGfxScene* scene)
{
  m_scene = scene;
}


iTexture2D* GL4ForwardPointLightRenderer::GetColorTexture()
{
    if (m_pointLightShadowMap[0])
    {
        return m_pointLightShadowMap[0]->GetColorTexture(0);
    }
    return nullptr;
}

iTexture2D* GL4ForwardPointLightRenderer::GetDepthTexture()
{
    if (m_pointLightShadowMap[0])
    {
        return m_pointLightShadowMap[0]->GetDepthTexture();
    }
    return nullptr;
}



}