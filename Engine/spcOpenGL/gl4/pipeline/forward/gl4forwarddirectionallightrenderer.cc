

#include <spcOpenGL/gl4/pipeline/forward/gl4forwarddirectionallightrenderer.hh>
#include <spcOpenGL/gl4/gl4directionallight.hh>
#include <spcOpenGL/gl4/gl4rendertarget2darray.hh>

#include <spcCore/settings.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/isampler.hh>
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/math/clipper/sphereclipper.hh>

#include <algorithm>

namespace spc::opengl
{

GL4ForwardDirectionalLightRenderer::GL4ForwardDirectionalLightRenderer()
  : m_device(nullptr)
  , m_shadowMapColorSampler(nullptr)
  , m_shadowMapDepthSampler(nullptr)
{

}

GL4ForwardDirectionalLightRenderer::~GL4ForwardDirectionalLightRenderer()
{
  SPC_RELEASE(m_shadowMapColorSampler);
  SPC_RELEASE(m_shadowMapDepthSampler);
}

void GL4ForwardDirectionalLightRenderer::Initialize(Settings &settings)
{
  m_directionalLightShadowMapSize = settings.GetInt("directional_light.shadow_map.size", 1024);
  std::string filter = settings.GetText("directional_light.shadow_map.filter", "PCF");
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

void GL4ForwardDirectionalLightRenderer::SetDevice(iDevice* device)
{
  m_device = device;
}

void GL4ForwardDirectionalLightRenderer::SetScene(GfxScene* scene)
{
  m_scene = scene;
}

iTexture2DArray* GL4ForwardDirectionalLightRenderer::GetColorTexture()
{
  return !m_directionalLightShadowMap.empty() && m_directionalLightShadowMap[0] 
    ? m_directionalLightShadowMap[0]->GetColorTexture(0) 
    : nullptr;
}

iTexture2DArray* GL4ForwardDirectionalLightRenderer::GetDepthTexture()
{
  return !m_directionalLightShadowMap.empty() && m_directionalLightShadowMap[0]
    ? m_directionalLightShadowMap[0]->GetDepthTexture()
    : nullptr;
}

void GL4ForwardDirectionalLightRenderer::Clear()
{
  m_shadowDirectionalLights.clear();
}

void GL4ForwardDirectionalLightRenderer::Add(GL4DirectionalLight* directionalLight)
{
  if (directionalLight)
  {
    m_shadowDirectionalLights.push_back(directionalLight);
  }
}

Size GL4ForwardDirectionalLightRenderer::RenderShadowMaps(Size maxShadowLights)
{
  SortLights();
  Size i = 0;
  for (auto directionalLight : m_shadowDirectionalLights)
  {
    if (i >= maxShadowLights)
    {
      break;
    }

    GL4RenderTarget2DArray* shadowMap = GetDirectionalLightShadowMap(i);
    if (!shadowMap)
    {
      break;
    }
    RenderDirectionalShadowMaps(directionalLight, shadowMap);
    m_device->SetDirectionalLightShadowMap(
      directionalLight,
      3,
      shadowMap->GetColorTexture(0),
      shadowMap->GetDepthTexture(),
      m_shadowMatrices,
      directionalLight->GetShadowMapBias()
    );

  }
  return i;
}


void GL4ForwardDirectionalLightRenderer::SortLights()
{

  std::sort(m_shadowDirectionalLights.begin(), m_shadowDirectionalLights.end(),
    [](GL4DirectionalLight* light0, GL4DirectionalLight* light1) {
      return light0->GetIntensity() > light1->GetIntensity();
    });
}

GL4RenderTarget2DArray* GL4ForwardDirectionalLightRenderer::GetDirectionalLightShadowMap(Size idx)
{


  if (m_directionalLightShadowMap.size() <= idx)
  {
    iRenderTarget2DArray::Descriptor desc{};
    desc.Width = (UInt16)m_directionalLightShadowMapSize;
    desc.Height = (UInt16)m_directionalLightShadowMapSize;
    desc.Layer = 3;
    GL4RenderTarget2DArray* shadowRenderTarget = static_cast<GL4RenderTarget2DArray*>(m_device->CreateRenderTarget(desc));


    if (m_shadowMapFilter == ShadowMapFilter::VSM || true)
    {
      iTexture2DArray::Descriptor colorDesc{};
      colorDesc.Width = desc.Width;
      colorDesc.Height = desc.Height;
      colorDesc.Layers = desc.Layer;
      colorDesc.Format = ePF_RGBA;
      colorDesc.MipMaps = false;
      iTexture2DArray* colorTexture = m_device->CreateTexture(colorDesc);
      colorTexture->SetSampler(GetShadowMapColorSampler());
      shadowRenderTarget->AddColorTexture(colorTexture);
      colorTexture->Release();
    }

    iTexture2DArray::Descriptor depthDesc{};
    depthDesc.Width = desc.Width;
    depthDesc.Height = desc.Height;
    depthDesc.Layers = desc.Layer;
    depthDesc.Format = ePF_Depth;
    depthDesc.MipMaps = false;
    iTexture2DArray* depthTexture = m_device->CreateTexture(depthDesc);
    depthTexture->SetSampler(GetShadowMapDepthSampler());
    shadowRenderTarget->SetDepthTexture(depthTexture);
    depthTexture->Release();

    if (!shadowRenderTarget->Compile())
    {
      shadowRenderTarget->Release();
      shadowRenderTarget = nullptr;
      return nullptr;
    }
    else
    {
      m_directionalLightShadowMap.push_back(shadowRenderTarget);
    }
  }
  return m_directionalLightShadowMap[idx];
}

iSampler *GL4ForwardDirectionalLightRenderer::GetShadowMapColorSampler()
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


iSampler* GL4ForwardDirectionalLightRenderer::GetShadowMapDepthSampler()
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
    /*
    m_shadowMapDepthSampler->SetTextureCompareMode(eTCM_CompareToR);
    m_shadowMapDepthSampler->SetTextureCompareFunc(eCF_LessOrEqual);
    */
  }
  return m_shadowMapDepthSampler;
}



void GL4ForwardDirectionalLightRenderer::RenderDirectionalShadowMaps(GL4DirectionalLight* directionalLight, GL4RenderTarget2DArray* shadowMap)
{
  m_device->SetRenderTarget(shadowMap);
  m_device->Clear(true, Color4f(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, false, 0);


  float near = -20.0;
  float far = 20.0;
  Matrix4f projection;
  m_device->GetOrthographicProjection(-10.0f, 10.0f, -10.0f, 10.0, -20.0f, 20.0f, projection);
  Matrix4f projections[] = {
    projection,
    projection,
    projection
  };
  Vector3f pos(0.0f, 0.0f, 0.0f);
  Matrix4f views[3];
  views[0].SetLookAt(pos, pos - directionalLight->GetDirection(), Vector3f(0, 1, 0));
  views[1].SetLookAt(pos, pos - directionalLight->GetDirection(), Vector3f(0, 1, 0));
  views[2].SetLookAt(pos, pos - directionalLight->GetDirection(), Vector3f(0, 1, 0));


  m_device->SetShadowMapProjectionMatrices(projections, 3);
  m_device->SetShadowMapViewMatrices(views, 3);

  m_scene->ScanMeshes(nullptr, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
    [this](GfxMesh* mesh)
    {
      mesh->RenderUnlit(m_device, eRP_ShadowPSSM);
    }
  );
}


}