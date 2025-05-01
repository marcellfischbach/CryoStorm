


#include <csOpenGL/gl4/pipeline/forward/csGL4ForwardDirectionalLightRenderer.hh>
#include <csOpenGL/gl4/pipeline/csGL4MeshSorter.hh>
#include <csOpenGL/gl4/csGL4Device.hh>
#include <csOpenGL/gl4/csGL4DirectionalLight.hh>
#include <csOpenGL/gl4/csGL4RenderTarget2D.hh>
#include <csOpenGL/gl4/csGL4RenderTarget2DArray.hh>
#include <csOpenGL/gl4/csGL4Texture2DArray.hh>

#include <csCore/csSettings.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/scene/csGfxMesh.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>
#include <csCore/math/csMath.hh>
#include <csCore/math/clipper/csCameraClipper.hh>
#include <csCore/resource/csAssetManager.hh>

#include <algorithm>

namespace cs::opengl
{

csGL4ForwardDirectionalLightRenderer::csGL4ForwardDirectionalLightRenderer()
{
  for (unsigned i = 0; i < MaxLights; i++)
  {
    m_directionalLightShadowMap[i] = nullptr;
  }
}

csGL4ForwardDirectionalLightRenderer::~csGL4ForwardDirectionalLightRenderer()
{
}

void csGL4ForwardDirectionalLightRenderer::Initialize()
{
  m_pssmRenderer.Initialize();
}

void csGL4ForwardDirectionalLightRenderer::Clear()
{
  m_shadowDirectionalLights.clear();
}

void csGL4ForwardDirectionalLightRenderer::Add(csGL4DirectionalLight *directionalLight)
{
  if (directionalLight)
  {
    m_shadowDirectionalLights.push_back(directionalLight);
  }
}

void csGL4ForwardDirectionalLightRenderer::RenderShadowMaps(const csCamera &camera,
                                                            const csProjector &projector)
{
  m_pssmRenderer.SetDevice(m_device);
  m_pssmRenderer.SetScene(m_scene);
  m_pssmRenderer.SetDepthBuffer(m_depthBuffer);


  size_t lightIdx = 0;
  SortLights();
  for (auto directionalLight: m_shadowDirectionalLights)
  {
    if (!m_device->MoreShadowMapsPossible())
    {
      break;
    }

    RenderShadow(directionalLight, camera, projector, lightIdx++);
  }
}

void csGL4ForwardDirectionalLightRenderer::SortLights()
{

  std::sort(m_shadowDirectionalLights.begin(), m_shadowDirectionalLights.end(),
            [](csGL4DirectionalLight *light0, csGL4DirectionalLight *light1)
            {
              return light0->GetIntensity() > light1->GetIntensity();
            });
}

void csGL4ForwardDirectionalLightRenderer::RenderShadow(csGL4DirectionalLight *directionalLight,
                                                        const csCamera &camera,
                                                        const csProjector &projector,
                                                        size_t lightIdx)
{
  csGL4RenderTarget2D         *target = GetDirectionalLightShadowMap(lightIdx);
  csGL4PSSMShadowBufferObject *sbo    = GetDirectionalLightShadowBuffer(lightIdx);

  if (target && sbo)
  {
    m_pssmRenderer.SetShadowMap(target);
    m_pssmRenderer.SetShadowBuffer(*sbo);
    m_pssmRenderer.RenderShadow(directionalLight, camera, projector);

    m_device->AddDirectionalLightShadow(directionalLight, target->GetColorTexture(0));
//    m_device->SetLightShadowMap(directionalLight, target->GetColorTexture(0));
  }
}


void csGL4ForwardDirectionalLightRenderer::ApplyShadowMapToDevice(csGL4DirectionalLight *directionalLight, size_t lightIdx)
{
//  if (lightIdx >= MaxLights)
//  {
//    return;
//  }
//
//  GL4RenderTarget2D *target = m_directionalLightShadowMap[lightIdx];
//  if (!target)
//  {
//    return;
//  }
//
//  m_device->SetLightShadowMap(directionalLight, target->GetColorTexture(0));
}

csGL4RenderTarget2D *csGL4ForwardDirectionalLightRenderer::GetDirectionalLightShadowMap(size_t lightIdx)
{
  if (lightIdx >= MaxLights)
  {
    return nullptr;
  }

  csGL4RenderTarget2D *target = m_directionalLightShadowMap[lightIdx];
  if (m_pssmRenderer.IsShadowMapValid(m_directionalLightShadowMap[lightIdx]))
  {
    return target;
  }

  m_directionalLightShadowMap[lightIdx] = m_pssmRenderer.CreateDirectionalLightShadowMap();
  return m_directionalLightShadowMap[lightIdx];
}

csGL4PSSMShadowBufferObject *csGL4ForwardDirectionalLightRenderer::GetDirectionalLightShadowBuffer(size_t lightIdx)
{
  if (lightIdx >= MaxLights)
  {
    return nullptr;
  }

  csGL4PSSMShadowBufferObject &shadowBuffer = m_directionalLightShadowBuffer[lightIdx];
  if (!m_pssmRenderer.IsShadowBufferValid(shadowBuffer))
  {
    m_directionalLightShadowBuffer[lightIdx] = m_pssmRenderer.CreateDirectionalLightShadowBuffer();
  }

  return &m_directionalLightShadowBuffer[lightIdx];
}


void csGL4ForwardDirectionalLightRenderer::SetDevice(iDevice *device)
{
  m_device = csQueryClass<csGL4Device>(device);
}

void csGL4ForwardDirectionalLightRenderer::SetScene(iGfxScene *scene)
{
  m_scene = scene;
}

void csGL4ForwardDirectionalLightRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  m_depthBuffer = depthBuffer;
}


iTexture2D *csGL4ForwardDirectionalLightRenderer::GetColorTexture()
{
  if (!m_directionalLightShadowMap.empty())
  {
    return m_directionalLightShadowMap[0]->GetColorTexture(0);
  }
  return nullptr;
}

iTexture2D *csGL4ForwardDirectionalLightRenderer::GetDepthTexture()
{
  if (!m_directionalLightShadowMap.empty())
  {
    return m_directionalLightShadowMap[0]->GetDepthTexture();
  }
  return nullptr;
}


}