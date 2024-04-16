

#include <ceOpenGL/gl4/pipeline/forward/gl4forwarddirectionallightrenderer.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardmeshsorter.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4directionallight.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceOpenGL/gl4/gl4rendertarget2darray.hh>

#include <ceCore/settings.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/isampler.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>
#include <ceCore/math/math.hh>
#include <ceCore/math/clipper/cameraclipper.hh>
#include <ceCore/resource/assetmanager.hh>

#include <algorithm>

namespace ce::opengl
{

GL4ForwardDirectionalLightRenderer::GL4ForwardDirectionalLightRenderer()
{
  for (unsigned i = 0; i < MaxLights; i++)
  {
    m_directionalLightShadowMap[i] = nullptr;
  }
}

GL4ForwardDirectionalLightRenderer::~GL4ForwardDirectionalLightRenderer()
{
}

void GL4ForwardDirectionalLightRenderer::Initialize()
{
  m_pssmRenderer.Initialize();
}

void GL4ForwardDirectionalLightRenderer::Clear()
{
  m_shadowDirectionalLights.clear();
}

void GL4ForwardDirectionalLightRenderer::Add(GL4DirectionalLight *directionalLight)
{
  if (directionalLight)
  {
    m_shadowDirectionalLights.push_back(directionalLight);
  }
}

void GL4ForwardDirectionalLightRenderer::RenderShadowMaps(const Camera &camera,
                                                          const Projector &projector)
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

void GL4ForwardDirectionalLightRenderer::SortLights()
{

  std::sort(m_shadowDirectionalLights.begin(), m_shadowDirectionalLights.end(),
            [](GL4DirectionalLight *light0, GL4DirectionalLight *light1) {
              return light0->GetIntensity() > light1->GetIntensity();
            });
}

void GL4ForwardDirectionalLightRenderer::RenderShadow(GL4DirectionalLight *directionalLight,
                                                      const Camera &camera,
                                                      const Projector &projector,
                                                      size_t lightIdx)
{
  GL4RenderTarget2D *target = GetDirectionalLightShadowMap(lightIdx);
  if (target)
  {
    m_pssmRenderer.SetShadowMap(target);
    m_pssmRenderer.RenderShadow(directionalLight, camera, projector);

    ApplyShadowMapToDevice(directionalLight, lightIdx);
  }
}


void GL4ForwardDirectionalLightRenderer::ApplyShadowMapToDevice(GL4DirectionalLight *directionalLight, size_t lightIdx)
{
  if (lightIdx >= MaxLights)
  {
    return;
  }

  GL4RenderTarget2D *target = m_directionalLightShadowMap[lightIdx];
  if (!target)
  {
    return;
  }

  m_device->SetLightShadowMap(directionalLight, target->GetColorTexture(0));
}

GL4RenderTarget2D *GL4ForwardDirectionalLightRenderer::GetDirectionalLightShadowMap(size_t lightIdx)
{
  if (lightIdx >= MaxLights)
  {
    return nullptr;
  }

  GL4RenderTarget2D *target = m_directionalLightShadowMap[lightIdx];
  if (m_pssmRenderer.IsShadowMapValid (target))
  {
    return target;
  }

  target = m_pssmRenderer.CreateDirectionalLightShadowMap();
  CE_SET(m_directionalLightShadowMap[lightIdx], target);
  return target;
}


void GL4ForwardDirectionalLightRenderer::SetDevice(iDevice *device)
{
  m_device = QueryClass<GL4Device>(device);
}

void GL4ForwardDirectionalLightRenderer::SetScene(iGfxScene *scene)
{
  m_scene = scene;
}

void GL4ForwardDirectionalLightRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  m_depthBuffer                     = depthBuffer;
}


iTexture2D *GL4ForwardDirectionalLightRenderer::GetColorTexture()
{
  if (!m_directionalLightShadowMap.empty())
  {
    return m_directionalLightShadowMap[0]->GetColorTexture(0);
  }
  return nullptr;
}

iTexture2D *GL4ForwardDirectionalLightRenderer::GetDepthTexture()
{
  if (!m_directionalLightShadowMap.empty())
  {
    return m_directionalLightShadowMap[0]->GetDepthTexture();
  }
  return nullptr;
}


}