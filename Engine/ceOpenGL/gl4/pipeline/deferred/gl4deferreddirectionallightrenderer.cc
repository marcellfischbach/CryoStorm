
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferreddirectionallightrenderer.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/gbuffer.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>


namespace ce::opengl
{


GL4DeferredDirectionalLightRenderer::GL4DeferredDirectionalLightRenderer()
: m_device(nullptr)
{

}

void GL4DeferredDirectionalLightRenderer::Initialize()
{

  m_lightShader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/deferred/directional_light_deferred.shader"));

  if (m_lightShader)
  {
    m_lightShaderLightColor = m_lightShader->GetShaderAttribute("LightColor");
    m_lightShaderNegLightDirection = m_lightShader->GetShaderAttribute("NegLightDirection");
    m_lightShaderDiffuseRoughness = m_lightShader->GetShaderAttribute("DiffuseRoughness");
    m_lightShaderDepth = m_lightShader->GetShaderAttribute("Depth");
    m_lightShaderNormal = m_lightShader->GetShaderAttribute("Normal");
  }

  printf ("\n");
}

void GL4DeferredDirectionalLightRenderer::SetDevice(iDevice *device)
{
  m_device = device;
}

void GL4DeferredDirectionalLightRenderer::Render(iRenderTarget2D *target, GBuffer *gBuffer, iDirectionalLight *light)
{
  m_device->ResetTextures();
//  m_device->SetRenderTarget(target);
//  m_device->SetRenderBuffer(0);
//  m_device->SetDepthTest(false);
//  m_device->SetDepthWrite(false);
//  m_device->SetColorWrite(true, true, true, true);
//  m_device->SetBlendFactor(eBlendFactor::One, eBlendFactor::One);
//  m_device->SetBlending(true);
  m_device->SetShader(m_lightShader);


  if (m_lightShaderDiffuseRoughness)
  {
    eTextureUnit unit = m_device->BindTexture(gBuffer->GetDiffuseRoughness());
    m_lightShaderDiffuseRoughness->Bind(unit);
  }
  if (m_lightShaderDepth)
  {
    eTextureUnit unit = m_device->BindTexture(gBuffer->GetDepth());
    m_lightShaderDepth->Bind(unit);
  }
  if (m_lightShaderNormal)
  {
    eTextureUnit unit = m_device->BindTexture(gBuffer->GetNormal());
    m_lightShaderNormal->Bind(unit);
  }
  if (m_lightShaderNegLightDirection)
  {
    m_lightShaderNegLightDirection->Bind(-light->GetDirection());
  }
  if (m_lightShaderLightColor)
  {
    m_lightShaderLightColor->Bind(light->GetColor());
  }

  m_device->RenderFullscreen();
}

}
