

#include <spcOpenGL/gl4/pipeline/forward/gl4forwardpointlightrenderer.hh>
#include <spcOpenGL/gl4/gl4pointlight.hh>
#include <spcOpenGL/gl4/gl4rendertargetcube.hh>
#include <spcOpenGL/gl4/gl4texturecube.hh>

#include <spcCore/settings.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/isampler.hh>
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/math/clipper/multiplaneclipper.hh>
#include <spcCore/math/clipper/sphereclipper.hh>

#include <algorithm>
#include <GL/glew.h>
namespace spc::opengl
{

GL4ForwardPointLightRenderer::GL4ForwardPointLightRenderer()
  : m_device(nullptr)
  , m_shadowMapColorSampler(nullptr)
  , m_shadowMapDepthSampler(nullptr)
{

}

GL4ForwardPointLightRenderer::~GL4ForwardPointLightRenderer()
{
  SPC_RELEASE(m_shadowMapColorSampler);
  SPC_RELEASE(m_shadowMapDepthSampler);
}

void GL4ForwardPointLightRenderer::Initialize(Settings &settings)
{
  m_pointLightShadowMapSize = settings.GetInt("point_light.shadow_map.size", 1024);
  std::string filter = settings.GetText("point_light.shadow_map.filter", "PCF");
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
}

void GL4ForwardPointLightRenderer::SetDevice(iDevice* device)
{
  m_device = device;
}

void GL4ForwardPointLightRenderer::SetScene(GfxScene* scene)
{
  m_scene = scene;
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

Size GL4ForwardPointLightRenderer::RenderShadowMaps(Size maxShadowLights)
{
  SortLights();
  Size i = 0;
  for (auto pointLight : m_shadowPointLights)
  {
    if (i >= maxShadowLights)
    {
      break;
    }

    GL4RenderTargetCube* shadowMap = GetPointLightShadowMap(i);
    if (!shadowMap)
    {
      break;
    }
    //RenderPointShadowMaps(pointLight, shadowMap);
    RenderPointShadowMapsStraight(pointLight, shadowMap);
    m_device->SetPointLightShadowMap(
      pointLight,
      shadowMap->GetColorTexture(0),
      shadowMap->GetDepthTexture(),
      0.1f,
      pointLight->GetRange(),
      pointLight->GetShadowMapBias()
    );
    i++;
  }
  return i;
}


void GL4ForwardPointLightRenderer::SortLights()
{

  std::sort(m_shadowPointLights.begin(), m_shadowPointLights.end(),
    [](GL4PointLight* light0, GL4PointLight* light1) {
      return light0->GetIntensity() > light1->GetIntensity();
    });
}

GL4RenderTargetCube* GL4ForwardPointLightRenderer::GetPointLightShadowMap(Size idx)
{


  if (m_pointLightShadowMap.size() <= idx)
  {
    iRenderTargetCube::Descriptor desc{};
    desc.Size = static_cast<UInt16>(m_pointLightShadowMapSize);
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
  m_scene->ScanMeshes(&clipper, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
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


  for (int i=0; i<6; i++)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_shadow);
    
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


    //SphereClipper clipper(pos, pointLight->GetRange());
    MultiPlaneClipper clipper(viewsInv[i], projectionInv, false, true);

    m_scene->ScanMeshes(&clipper, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
                        [this](GfxMesh *mesh) {
                          mesh->RenderUnlit(m_device, eRP_Shadow);
                        }
    );
    glColorMask(true, true, true, true);
  }
  m_device->SetRenderTarget(nullptr);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}