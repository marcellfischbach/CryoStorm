

#include <ceOpenGL/gl4/pipeline/forward/gl4forwardshadowmapfilter.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/shading/gl4program.hh>
#include <ceOpenGL/gl4/shading/gl4shaderattribute.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceCore/resource/assetmanager.hh>

namespace ce::opengl
{


GL4ForwardShadowMapFilter::GL4ForwardShadowMapFilter()
{

}


GL4ForwardShadowMapFilter::~GL4ForwardShadowMapFilter()
{

}

bool GL4ForwardShadowMapFilter::Initialize(const Vector2f &distance, float radius, float samples)
{
  m_distance = distance;
  m_radius = radius;
  m_samples = samples;


  m_shadowMapFilterShader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/forward/shadow_map_filter.shader")
  );
  if (!m_shadowMapFilterShader)
  {
    return false;
  }


  m_attrFilterShadowMap = m_shadowMapFilterShader->GetShaderAttribute("ShadowMap");
  m_attrFilterDepthBuffer = m_shadowMapFilterShader->GetShaderAttribute("DepthBuffer");
  m_attrFilterDistance = m_shadowMapFilterShader->GetShaderAttribute("FilterDistance");
  m_attrFilterRadius = m_shadowMapFilterShader->GetShaderAttribute("FilterRadius");
  m_attrFilterSamples = m_shadowMapFilterShader->GetShaderAttribute("FilterSamples");
  m_attrScreenAspect = m_shadowMapFilterShader->GetShaderAttribute("ScreenAspect");

  return true;
}


void GL4ForwardShadowMapFilter::Render(GL4Device *device,
                                       iTexture2D *depthBuffer,
                                       iTexture2D *shadowMap,
                                       GL4RenderTarget2D *target)
{
  device->SetRenderTarget(target);
  device->SetDepthWrite(false);
  device->SetDepthTest(false);
  device->SetColorWrite(true, true, true, true);

  device->SetShader(m_shadowMapFilterShader);
  if (m_attrFilterShadowMap)
  {
    eTextureUnit unit = device->BindTexture(shadowMap);
    m_attrFilterShadowMap->Bind(unit);
  }
  if (m_attrFilterDepthBuffer)
  {
    eTextureUnit unit = device->BindTexture(depthBuffer);
    m_attrFilterDepthBuffer->Bind(unit);
  }

  if (m_attrFilterDistance)
  {
    m_attrFilterDistance->Bind(m_distance);
  }

  if (m_attrFilterRadius)
  {
    m_attrFilterRadius->Bind(m_radius);
  }

  if (m_attrFilterSamples)
  {
    m_attrFilterSamples->Bind(m_samples);
  }

  if (m_attrScreenAspect)
  {
    m_attrScreenAspect->Bind((float)target->GetHeight() / (float)target->GetWidth());
  }

  device->BindMatrices();

  device->RenderFullscreen();
}


}
