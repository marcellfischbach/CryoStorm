

#include <ceOpenGL/gl4/pipeline/pssm/gl4pssmfilter.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/shading/gl4program.hh>
#include <ceOpenGL/gl4/shading/gl4shaderattribute.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceCore/resource/csAssetManager.hh>

namespace cryo::opengl
{


GL4PSSMFilter::GL4PSSMFilter()
{

}


GL4PSSMFilter::~GL4PSSMFilter()
{

}

bool GL4PSSMFilter::Initialize(const csVector2f &distance, float radius, float samplesFactor, float maxSampleDistance)
{
  m_distance          = distance;
  m_radius            = radius;
  m_samplesFactor     = samplesFactor;
  m_maxSampleDistance = maxSampleDistance;


  m_shadowMapFilterShader = csAssetManager::Get()->Get<iShader>(
      csResourceLocator("file://${engine}/opengl/gl4/pssm/directional_light_shadow_map_filter.shader")
  );
  if (!m_shadowMapFilterShader)
  {
    return false;
  }


  m_attrFilterShadowMap         = m_shadowMapFilterShader->GetShaderAttribute("ShadowMap");
  m_attrFilterDepthBuffer       = m_shadowMapFilterShader->GetShaderAttribute("DepthBuffer");
  m_attrFilterDistance          = m_shadowMapFilterShader->GetShaderAttribute("FilterDistance");
  m_attrFilterRadius            = m_shadowMapFilterShader->GetShaderAttribute("FilterRadius");
  m_attrFilterSamplesFactor     = m_shadowMapFilterShader->GetShaderAttribute("FilterSamplesFactor");
  m_attrFilterMaxSampleDistance = m_shadowMapFilterShader->GetShaderAttribute("FilterMaxSampleDistance");
  m_attrScreenAspect            = m_shadowMapFilterShader->GetShaderAttribute("ScreenAspect");
  m_attrScreenSize              = m_shadowMapFilterShader->GetShaderAttribute("ScreenSize");

  return true;
}


void GL4PSSMFilter::Render(GL4Device *device,
                           iTexture2D *depthBuffer,
                           iTexture2D *shadowMap,
                           GL4RenderTarget2D *target)
{
  device->SetRenderTarget(target);
  device->SetRenderBuffer(0);
  device->SetDepthWrite(false);
  device->SetDepthTest(false);
  device->SetColorWrite(true, true, true, true);

  device->SetShader(m_shadowMapFilterShader);
  device->ResetTextures();
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

  if (m_attrFilterSamplesFactor)
  {
    m_attrFilterSamplesFactor->Bind(m_samplesFactor);
  }
  if (m_attrFilterMaxSampleDistance)
  {
    m_attrFilterMaxSampleDistance->Bind(m_maxSampleDistance);
  }

  if (m_attrScreenAspect)
  {
    m_attrScreenAspect->Bind((float) target->GetHeight() / (float) target->GetWidth());
  }
  if (m_attrScreenSize)
  {
    m_attrScreenSize->Bind((float) target->GetWidth(), (float) target->GetHeight());
  }

  device->BindMatrices();
  device->BindStandardValues();

  device->RenderFullscreen();
}


}
