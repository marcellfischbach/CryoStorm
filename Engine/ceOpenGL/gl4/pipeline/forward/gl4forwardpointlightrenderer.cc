

#include <ceOpenGL/gl4/pipeline/forward/gl4forwardpointlightrenderer.hh>
#include <ceOpenGL/gl4/gl4pointlight.hh>
#include <ceOpenGL/gl4/gl4rendertargetcube.hh>
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
  : m_device(nullptr)
  , m_shadowMapColorSampler(nullptr)
  , m_shadowMapDepthSampler(nullptr)
{
  for (unsigned i = 0; i < MaxLights; i++)
  {
    m_directionalLightShadowMap[i] = nullptr;
  }
}

GL4ForwardPointLightRenderer::~GL4ForwardPointLightRenderer()
{
  CE_RELEASE(m_shadowMapColorSampler);
  CE_RELEASE(m_shadowMapDepthSampler);
}

void GL4ForwardPointLightRenderer::Initialize(Settings &settings)
{
  m_pointLightShadowBufferSize = settings.GetInt("point_light.shadow_map.size", 1024);
  std::string filter = settings.GetText("point_light.shadow_map.filter", "Plain");
  if (filter == std::string("Plain"))
  {
    m_shadowMapFilter = ShadowMapFilter::Plain;
  }
  else if (filter == std::string("PCF"))
  {
    m_shadowMapFilter = ShadowMapFilter::PCF;
  }
  else if (filter == std::string("VSM"))
  {
    m_shadowMapFilter = ShadowMapFilter::VSM;
  }


  m_shadowMappingShader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/forward/directional_light_shadow_map.shader"));
  if (m_shadowMappingShader)
  {
    m_attrLightPosition = m_shadowMappingShader->GetShaderAttribute("LightPosition");
    m_attrMappingBias = m_shadowMappingShader->GetShaderAttribute("MappingBias");
    m_attrShadowBuffer = m_shadowMappingShader->GetShaderAttribute("ShadowBuffer");
    m_attrDepthBuffer = m_shadowMappingShader->GetShaderAttribute("DepthBuffer");
  }

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





  float near = 0.1f;
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
    m_device->SetColorWrite(false, false, false, false)

    m_device->SetViewport(0, 0, m_pointLightShadowBufferSize, m_pointLightShadowBufferSize);
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

}

void GL4ForwardPointLightRenderer::FilterShadowMap(size_t lightIdx)
{

}

void GL4ForwardPointLightRenderer::ApplyShadowMapToDevice(GL4PointLight *pointLight, size_t lightIdx)
{

}















GL4RenderTargetCube* GL4ForwardPointLightRenderer::GetPointLightShadowMap(Size idx)
{


  if (m_pointLightShadowMap.size() <= idx)
  {
    iRenderTargetCube::Descriptor desc{};
    desc.Size = static_cast<uint16_t>(m_pointLightShadowBufferSize);
    GL4RenderTargetCube* cubeMap = static_cast<GL4RenderTargetCube*>(m_device->CreateRenderTarget(desc));


    if (m_shadowMapFilter == ShadowMapFilter::VSM)
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
    if (m_shadowMapFilter == ShadowMapFilter::PCF)
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



void GL4ForwardPointLightRenderer::SetDevice(iDevice* device)
{
  m_device = device;
}

void GL4ForwardPointLightRenderer::SetScene(iGfxScene* scene)
{
  m_scene = scene;
}


}