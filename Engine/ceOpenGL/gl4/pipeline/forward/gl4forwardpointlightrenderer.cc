

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
}

void GL4ForwardPointLightRenderer::Initialize()
{
  m_shadowRenderer.Initialize();
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
  GL4RenderTarget2D *target = GetPointLightShadowMap(lightIdx);
  if (target)
  {
    m_shadowRenderer.SetShadowMap(target);
    m_shadowRenderer.RenderShadow(pointLight, camera, projector);

    ApplyShadowMapToDevice(pointLight, lightIdx);
  }
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


GL4RenderTarget2D* GL4ForwardPointLightRenderer::GetPointLightShadowMap(Size lightIdx)
{
  if (lightIdx >= MaxLights)
  {
    return nullptr;
  }

  GL4RenderTarget2D *target = m_pointLightShadowMap[lightIdx];
  if (m_shadowRenderer.IsShadowMapValid (target))
  {
    return target;
  }

  target = m_shadowRenderer.CreateShadowMap();
  CE_SET(m_pointLightShadowMap[lightIdx], target);
  return target;
}


void GL4ForwardPointLightRenderer::SetDevice(iDevice* device)
{
  m_device = QueryClass<GL4Device>(device);
  m_shadowRenderer.SetDevice(m_device);
}


void GL4ForwardPointLightRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  m_depthBuffer = depthBuffer;
  m_shadowRenderer.SetDepthBuffer(m_depthBuffer);
}



void GL4ForwardPointLightRenderer::SetScene(iGfxScene* scene)
{
  m_scene = scene;
  m_shadowRenderer.SetScene(m_scene);
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