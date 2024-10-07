

#include <csOpenGL/gl4/pipeline/forward/csGL4ForwardPointLightRenderer.hh>
#include <csOpenGL/gl4/csGL4Device.hh>
#include <csOpenGL/gl4/csGL4PointLight.hh>
#include <csOpenGL/gl4/csGL4RenderTargetCube.hh>
#include <csOpenGL/gl4/csGL4RenderTarget2D.hh>
#include <csOpenGL/gl4/csGL4TextureCube.hh>

#include <csCore/csSettings.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>
#include <csCore/graphics/scene/csGfxMesh.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/math/clipper/csCameraClipper.hh>
#include <csCore/math/clipper/csMultiPlaneClipper.hh>
#include <csCore/math/clipper/csSphereClipper.hh>
#include <csCore/resource/csAssetManager.hh>

#include <algorithm>
#include <GL/glew.h>



namespace cs::opengl
{

csGL4ForwardPointLightRenderer::csGL4ForwardPointLightRenderer()
{
  for (unsigned i = 0; i < MaxLights; i++)
  {
    m_pointLightShadowMap[i] = nullptr;
  }
}

csGL4ForwardPointLightRenderer::~csGL4ForwardPointLightRenderer()
{
}

void csGL4ForwardPointLightRenderer::Initialize()
{
  m_shadowRenderer.Initialize();
}

void csGL4ForwardPointLightRenderer::Clear()
{
  m_shadowPointLights.clear();
}

void csGL4ForwardPointLightRenderer::Add(csGL4PointLight* pointLight)
{
  if (pointLight)
  {
    m_shadowPointLights.push_back(pointLight);
  }
}

void csGL4ForwardPointLightRenderer::RenderShadowMaps(const csCamera &camera,
                                                      const csProjector &projector)
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


void csGL4ForwardPointLightRenderer::SortLights()
{

  std::sort(m_shadowPointLights.begin(), m_shadowPointLights.end(),
    [](csGL4PointLight* light0, csGL4PointLight* light1) {
      return light0->GetIntensity() > light1->GetIntensity();
    });
}


void csGL4ForwardPointLightRenderer::RenderShadow(csGL4PointLight *pointLight,
                                                  const csCamera &camera,
                                                  const csProjector &projector,
                                                  size_t lightIdx)
{
  csGL4RenderTarget2D *target = GetPointLightShadowMap(lightIdx);
  if (target)
  {
    m_shadowRenderer.SetShadowMap(target);
    m_shadowRenderer.RenderShadow(pointLight, camera, projector);

    ApplyShadowMapToDevice(pointLight, lightIdx);
  }
}



void csGL4ForwardPointLightRenderer::ApplyShadowMapToDevice(csGL4PointLight *pointLight, size_t lightIdx)
{
//    if (lightIdx >= MaxLights)
//    {
//        return;
//    }
//
//    GL4RenderTarget2D* target = m_pointLightShadowMap[lightIdx];
//    if (!target)
//    {
//        return;
//    }

//    m_device->SetLightShadowMap(pointLight, target->GetColorTexture(0));
}


csGL4RenderTarget2D* csGL4ForwardPointLightRenderer::GetPointLightShadowMap(Size lightIdx)
{
  if (lightIdx >= MaxLights)
  {
    return nullptr;
  }

  csGL4RenderTarget2D *target = m_pointLightShadowMap[lightIdx];
  if (m_shadowRenderer.IsShadowMapValid (target))
  {
    return target;
  }

  target = m_shadowRenderer.CreateShadowMap();
  CS_SET(m_pointLightShadowMap[lightIdx], target);
  return target;
}


void csGL4ForwardPointLightRenderer::SetDevice(iDevice* device)
{
  m_device = csQueryClass<csGL4Device>(device);
  m_shadowRenderer.SetDevice(m_device);
}


void csGL4ForwardPointLightRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  m_depthBuffer = depthBuffer;
  m_shadowRenderer.SetDepthBuffer(m_depthBuffer);
}



void csGL4ForwardPointLightRenderer::SetScene(iGfxScene* scene)
{
  m_scene = scene;
  m_shadowRenderer.SetScene(m_scene);
}


iTexture2D* csGL4ForwardPointLightRenderer::GetColorTexture()
{
    if (m_pointLightShadowMap[0])
    {
        return m_pointLightShadowMap[0]->GetColorTexture(0);
    }
    return nullptr;
}

iTexture2D* csGL4ForwardPointLightRenderer::GetDepthTexture()
{
    if (m_pointLightShadowMap[0])
    {
        return m_pointLightShadowMap[0]->GetDepthTexture();
    }
    return nullptr;
}



}