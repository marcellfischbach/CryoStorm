
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferreddirectionallightrenderer.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/gbuffer.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>

namespace ce::opengl
{

GL4DeferredDirectionalLightRenderer::GL4DeferredDirectionalLightRenderer()
{

}

bool GL4DeferredDirectionalLightRenderer::Initialize()
{
  m_nonShadow.m_shader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/deferred/directional_light_deferred_no_shadow.shader"));
  if (m_nonShadow.m_shader)
  {

    m_nonShadow.m_attrDiffuseRoughness       = m_nonShadow.m_shader->GetShaderAttribute("DiffuseRoughness");
    m_nonShadow.m_attrNormal                 = m_nonShadow.m_shader->GetShaderAttribute("Normal");
    m_nonShadow.m_attrDepth                  = m_nonShadow.m_shader->GetShaderAttribute("Depth");
    m_nonShadow.m_attrLightColor             = m_nonShadow.m_shader->GetShaderAttribute("LightColor");
    m_nonShadow.m_attrLightAmbientColor      = m_nonShadow.m_shader->GetShaderAttribute("LightAmbientColor");
    m_nonShadow.m_attrLightNegLightDirection = m_nonShadow.m_shader->GetShaderAttribute("NegLightDirection");
    m_nonShadow.m_attrCameraPosition         = m_nonShadow.m_shader->GetShaderAttribute("CameraPosition");
  }

  m_shadow.m_shader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/deferred/directional_light_deferred_shadow.shader"));
  if (m_shadow.m_shader)
  {

    m_shadow.m_attrDiffuseRoughness       = m_shadow.m_shader->GetShaderAttribute("DiffuseRoughness");
    m_shadow.m_attrNormal                 = m_shadow.m_shader->GetShaderAttribute("Normal");
    m_shadow.m_attrDepth                  = m_shadow.m_shader->GetShaderAttribute("Depth");
    m_shadow.m_attrShadowMap              = m_shadow.m_shader->GetShaderAttribute("ShadowMap");
    m_shadow.m_attrLightColor             = m_shadow.m_shader->GetShaderAttribute("LightColor");
    m_shadow.m_attrLightAmbientColor      = m_shadow.m_shader->GetShaderAttribute("LightAmbientColor");
    m_shadow.m_attrLightNegLightDirection = m_shadow.m_shader->GetShaderAttribute("NegLightDirection");
    m_shadow.m_attrCameraPosition         = m_shadow.m_shader->GetShaderAttribute("CameraPosition");
  }

  return true;
}


void GL4DeferredDirectionalLightRenderer::Render(ce::iDevice *device,
                                                 const Camera *camera,
                                                 const Projector *projector,
                                                 const GBuffer *gBuffer,
                                                 const ce::iDirectionalLight *light,
                                                 ce::iRenderTarget2D *target)
{
  device->SetRenderTarget(target);
  device->SetRenderBuffer(0);

  device->SetBlending(true);
  device->SetBlendFactor(eBlendFactor::One,
                         eBlendFactor::One,
                         eBlendFactor::One,
                         eBlendFactor::One
  );
  device->SetViewMatrix(camera->GetViewMatrix(), camera->GetViewMatrixInv());
  device->SetProjectionMatrix(projector->GetProjectionMatrix(device), projector->GetProjectionMatrixInv(device));

  device->SetShader(m_shader);
  if (m_attrDiffuseRoughness)
  {
    eTextureUnit unit = device->BindTexture(gBuffer->GetDiffuseRoughness());
    m_attrDiffuseRoughness->Bind(unit);
  }
  if (m_attrDepth)
  {
    eTextureUnit unit = device->BindTexture(gBuffer->GetDepth());
    m_attrDepth->Bind(unit);
  }

  if (m_attrNormal)
  {
    eTextureUnit unit = device->BindTexture(gBuffer->GetNormal());
    m_attrNormal->Bind(unit);
  }
  if (m_attrLightNegLightDirection)
  {
    m_attrLightNegLightDirection->Bind(-light->GetDirection());
  }
  if (m_attrLightColor)
  {
    m_attrLightColor->Bind(light->GetColor());
  }
  if (m_attrLightAmbientColor)
  {
    m_attrLightAmbientColor->Bind(Color4f());
  }
  if (m_attrCameraPosition)
  {
    m_attrCameraPosition->Bind(camera->GetEye());
  }

  static_cast<GL4Device *>(device)->BindMatrices();
  device->RenderFullscreen();

}


}
