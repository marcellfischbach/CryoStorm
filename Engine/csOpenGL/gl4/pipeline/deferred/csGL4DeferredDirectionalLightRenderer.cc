
#include <csOpenGL/gl4/pipeline/deferred/csGL4DeferredDirectionalLightRenderer.hh>
#include <csOpenGL/gl4/csGL4Device.hh>
#include <csOpenGL/gl4/csGL4DirectionalLight.hh>
#include <csOpenGL/gl4/csGL4RenderTarget2D.hh>
#include <csOpenGL/gl4/csGL4Texture2DArray.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/csGBuffer.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>


namespace cs::opengl
{


bool csGL4DeferredDirectionalLightRenderer::Initialize()
{
  m_pssmRenderer.Initialize();

  m_nonShadow.m_shader = csAssetManager::Get()->Get<iShader>(
      csAssetLocator("/graphics/gl4/deferred/directional_light_deferred_no_shadow.shader"));
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
      csAssetLocator("/graphics/gl4/deferred/directional_light_deferred_shadow.shader"));
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

  for (size_t i=0; i<m_shadowBuffers.size(); i++)
  {
    m_shadowBuffers[i] = nullptr;
  }

  return true;
}

void csGL4DeferredDirectionalLightRenderer::SetDevice(iDevice *device)
{
  m_device = csQueryClass<csGL4Device>(device);
}

void csGL4DeferredDirectionalLightRenderer::SetScene(cs::iGfxScene *scene)
{
  m_scene = scene;
}

void csGL4DeferredDirectionalLightRenderer::Render(const csCamera *camera,
                                                   const csProjector *projector,
                                                   const csGBuffer *gBuffer,
                                                   csGL4DirectionalLight *light,
                                                   iRenderTarget2D *target)
{
  LightRenderShader   *lrs       = &m_nonShadow;
  csGL4RenderTarget2D *shadowMap = nullptr;
  if (light->IsCastShadow())
  {

    m_pssmRenderer.SetDevice(m_device);
    m_pssmRenderer.SetScene(m_scene);
    m_pssmRenderer.SetDepthBuffer(gBuffer->GetDepth());
    shadowMap = GetShadowMap();
    m_pssmRenderer.SetShadowMap(shadowMap);

    iPSSMShadowBufferObject *sbo = GetShadowBuffer();
    m_pssmRenderer.SetShadowBuffer(sbo);
    m_pssmRenderer.RenderShadow(light, *camera, *projector);

    if (m_device->MoreShadowMapsPossible())
    {
      m_device->AddDirectionalLightShadow(light, shadowMap->GetColorTexture(0));
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

void csGL4DeferredDirectionalLightRenderer::ResetShadowBuffer()
{
  m_lightIdx = 0;
}

csGL4RenderTarget2D *csGL4DeferredDirectionalLightRenderer::GetShadowMap()
{
  if (m_device->MoreShadowMapsPossible() && m_lightIdx < m_shadowMaps.size())
  {
    csGL4RenderTarget2D *target = m_shadowMaps[m_lightIdx];
    if (m_pssmRenderer.IsShadowMapValid(target))
    {
      return target;
    }

    m_shadowMaps[m_lightIdx] = m_pssmRenderer.CreateDirectionalLightShadowMap();
    return m_shadowMaps[m_lightIdx];
  }
  else
  {

    if (m_pssmRenderer.IsShadowMapValid(m_shadowMap))
    {
      return m_shadowMap;
    }

    m_shadowMap = m_pssmRenderer.CreateDirectionalLightShadowMap();
    return m_shadowMap;
  }
}

iPSSMShadowBufferObject *csGL4DeferredDirectionalLightRenderer::GetShadowBuffer()
{
  if (m_device->MoreShadowMapsPossible() && m_lightIdx < m_shadowBuffers.size())
  {
    if (!m_pssmRenderer.IsShadowBufferValid(m_shadowBuffers[m_lightIdx]))
    {
      m_pssmRenderer.DeleteDirectionalLightShadowBuffer(m_shadowBuffers[m_lightIdx]);
      m_shadowBuffers[m_lightIdx] = m_pssmRenderer.CreateDirectionalLightShadowBuffer();
    }

    return m_shadowBuffers[m_lightIdx];
  }
  else
  {
    if (!m_pssmRenderer.IsShadowBufferValid(m_shadowBuffer))
    {
      m_pssmRenderer.DeleteDirectionalLightShadowBuffer(m_shadowBuffer);
      m_shadowBuffer = m_pssmRenderer.CreateDirectionalLightShadowBuffer();
    }

    return m_shadowBuffer;
  }
}

}
