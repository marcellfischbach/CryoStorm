
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferreddirectionallightrenderer.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4directionallight.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceOpenGL/gl4/gl4texture2darray.hh>
#include <ceCore/resource/csAssetManager.hh>
#include <ceCore/graphics/csCamera.hh>
#include <ceCore/graphics/iDevice.hh>
#include <ceCore/graphics/csGBuffer.hh>
#include <ceCore/graphics/csProjector.hh>
#include <ceCore/graphics/shading/iShader.hh>
#include <ceCore/graphics/shading/iShaderAttribute.hh>


namespace cryo::opengl
{


bool GL4DeferredDirectionalLightRenderer::Initialize()
{
  m_pssmRenderer.Initialize();

  m_nonShadow.m_shader = csAssetManager::Get()->Get<iShader>(
      csResourceLocator("file://${engine}/opengl/gl4/deferred/directional_light_deferred_no_shadow.shader"));
  if (m_nonShadow.m_shader)
  {
    m_nonShadow.m_attrDiffuseRoughness       = m_nonShadow.m_shader->GetShaderAttribute("DiffuseRoughness");
    m_nonShadow.m_attrNormal                 = m_nonShadow.m_shader->GetShaderAttribute("Normal");
    m_nonShadow.m_attrDepth                  = m_nonShadow.m_shader->GetShaderAttribute("Depth");
    m_nonShadow.m_attrShadowMap              = nullptr;
    m_nonShadow.m_attrLightColor             = m_nonShadow.m_shader->GetShaderAttribute("LightColor");
    m_nonShadow.m_attrLightAmbientColor      = m_nonShadow.m_shader->GetShaderAttribute("LightAmbientColor");
    m_nonShadow.m_attrLightNegLightDirection = m_nonShadow.m_shader->GetShaderAttribute("NegLightDirection");
    m_nonShadow.m_attrCameraPosition         = m_nonShadow.m_shader->GetShaderAttribute("CameraPosition");
  }

  m_shadow.m_shader = csAssetManager::Get()->Get<iShader>(
      csResourceLocator("file://${engine}/opengl/gl4/deferred/directional_light_deferred_shadow.shader"));
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

void GL4DeferredDirectionalLightRenderer::SetDevice(iDevice *device)
{
  m_device = csQueryClass<GL4Device>(device);
}

void GL4DeferredDirectionalLightRenderer::SetScene(cryo::iGfxScene *scene)
{
  m_scene = scene;
}

void GL4DeferredDirectionalLightRenderer::Render(const csCamera *camera,
                                                 const csProjector *projector,
                                                 const csGBuffer *gBuffer,
                                                 GL4DirectionalLight *light,
                                                 iRenderTarget2D *target)
{
  LightRenderShader *lrs       = &m_nonShadow;
  GL4RenderTarget2D *shadowMap = nullptr;
  if (light->IsCastShadow())
  {

    m_pssmRenderer.SetDevice(m_device);
    m_pssmRenderer.SetScene(m_scene);
    m_pssmRenderer.SetDepthBuffer(gBuffer->GetDepth());
    shadowMap = GetShadowMap();
    m_pssmRenderer.SetShadowMap(shadowMap);

    GL4PSSMShadowBufferObject &sbo = GetShadowBuffer();
    m_pssmRenderer.SetShadowBuffer(sbo);
    m_pssmRenderer.RenderShadow(light, *camera, *projector);

    if (m_device->MoreShadowMapsPossible())
    {
      m_device->AddDirectionalLightShadow(light,
                                          shadowMap->GetColorTexture(0),
                                          sbo.ShadowDepth,
                                          sbo.ShadowColor,
                                          m_pssmRenderer.GetSplits(),
                                          m_pssmRenderer.GetMatrices());
    }

    m_lightIdx++;
    lrs = &m_shadow;
  }


  m_device->ResetTextures();
  m_device->SetRenderTarget(target);
  m_device->SetRenderBuffer(0);

  m_device->SetBlending(true);
  m_device->SetBlendFactor(eBlendFactor::One,
                           eBlendFactor::One,
                           eBlendFactor::One,
                           eBlendFactor::One
  );
  m_device->SetViewMatrix(camera->GetViewMatrix(), camera->GetViewMatrixInv());
  m_device->SetProjectionMatrix(projector->GetProjectionMatrix(m_device), projector->GetProjectionMatrixInv(m_device));

  m_device->SetShader(lrs->m_shader);
  if (lrs->m_attrDiffuseRoughness)
  {
    eTextureUnit unit = m_device->BindTexture(gBuffer->GetDiffuseRoughness());
    lrs->m_attrDiffuseRoughness->Bind(unit);
  }
  if (lrs->m_attrDepth)
  {
    eTextureUnit unit = m_device->BindTexture(gBuffer->GetDepth());
    lrs->m_attrDepth->Bind(unit);
  }
  if (lrs->m_attrShadowMap)
  {
    eTextureUnit unit = m_device->BindTexture(shadowMap->GetColorTexture(0));
    lrs->m_attrShadowMap->Bind(unit);
  }

  if (lrs->m_attrNormal)
  {
    eTextureUnit unit = m_device->BindTexture(gBuffer->GetNormal());
    lrs->m_attrNormal->Bind(unit);
  }
  if (lrs->m_attrLightNegLightDirection)
  {
    lrs->m_attrLightNegLightDirection->Bind(-light->GetDirection());
  }
  if (lrs->m_attrLightColor)
  {
    lrs->m_attrLightColor->Bind(light->GetColor());
  }
  if (lrs->m_attrLightAmbientColor)
  {
    lrs->m_attrLightAmbientColor->Bind(csColor4f());
  }
  if (lrs->m_attrCameraPosition)
  {
    lrs->m_attrCameraPosition->Bind(camera->GetEye());
  }

  m_device->BindMatrices();
  m_device->RenderFullscreen();

}

void GL4DeferredDirectionalLightRenderer::ResetShadowBuffer()
{
  m_lightIdx = 0;
}

GL4RenderTarget2D *GL4DeferredDirectionalLightRenderer::GetShadowMap()
{
  if (m_device->MoreShadowMapsPossible() && m_lightIdx < m_shadowMaps.size())
  {
    GL4RenderTarget2D *target = m_shadowMaps[m_lightIdx];
    if (m_pssmRenderer.IsShadowMapValid(target))
    {
      return target;
    }

    target = m_pssmRenderer.CreateDirectionalLightShadowMap();
    CS_SET(m_shadowMaps[m_lightIdx], target);
    return target;
  }
  else
  {

    if (m_pssmRenderer.IsShadowMapValid(m_shadowMap))
    {
      return m_shadowMap;
    }

    GL4RenderTarget2D *target = m_pssmRenderer.CreateDirectionalLightShadowMap();
    CS_SET(m_shadowMap, target);
    return target;
  }
}

GL4PSSMShadowBufferObject &GL4DeferredDirectionalLightRenderer::GetShadowBuffer()
{
  if (m_device->MoreShadowMapsPossible() && m_lightIdx < m_shadowBuffers.size())
  {
    if (!m_pssmRenderer.IsShadowBufferValid(m_shadowBuffers[m_lightIdx]))
    {
      m_shadowBuffers[m_lightIdx] = m_pssmRenderer.CreateDirectionalLightShadowBuffer();
    }

    return m_shadowBuffers[m_lightIdx];
  }
  else
  {
    if (!m_pssmRenderer.IsShadowBufferValid(m_shadowBuffer))
    {
      m_shadowBuffer = m_pssmRenderer.CreateDirectionalLightShadowBuffer();
    }

    return m_shadowBuffer;
  }
}

}
