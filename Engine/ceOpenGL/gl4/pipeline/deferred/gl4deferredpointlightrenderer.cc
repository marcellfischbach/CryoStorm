
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferredpointlightrenderer.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4pointlight.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/gbuffer.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>


namespace ce::opengl
{


bool GL4DeferredPointLightRenderer::Initialize(Settings &settings)
{
  m_shadowRenderer.Initialize(settings);

  m_nonShadow.m_shader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/deferred/point_light_deferred_no_shadow.shader"));
  if (m_nonShadow.m_shader)
  {
    m_nonShadow.m_attrDiffuseRoughness  = m_nonShadow.m_shader->GetShaderAttribute("DiffuseRoughness");
    m_nonShadow.m_attrNormal            = m_nonShadow.m_shader->GetShaderAttribute("Normal");
    m_nonShadow.m_attrDepth             = m_nonShadow.m_shader->GetShaderAttribute("Depth");
    m_nonShadow.m_attrShadowMap         = nullptr;
    m_nonShadow.m_attrLightColor        = m_nonShadow.m_shader->GetShaderAttribute("LightColor");
    m_nonShadow.m_attrLightAmbientColor = m_nonShadow.m_shader->GetShaderAttribute("LightAmbientColor");
    m_nonShadow.m_attrLightPosition     = m_nonShadow.m_shader->GetShaderAttribute("LightPosition");
    m_nonShadow.m_attrLightRange        = m_nonShadow.m_shader->GetShaderAttribute("LightRange");
    m_nonShadow.m_attrCameraPosition    = m_nonShadow.m_shader->GetShaderAttribute("CameraPosition");
  }

  m_shadow.m_shader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/deferred/point_light_deferred_shadow.shader"));
  if (m_shadow.m_shader)
  {

    m_shadow.m_attrDiffuseRoughness  = m_shadow.m_shader->GetShaderAttribute("DiffuseRoughness");
    m_shadow.m_attrNormal            = m_shadow.m_shader->GetShaderAttribute("Normal");
    m_shadow.m_attrDepth             = m_shadow.m_shader->GetShaderAttribute("Depth");
    m_shadow.m_attrShadowMap         = m_shadow.m_shader->GetShaderAttribute("ShadowMap");
    m_shadow.m_attrLightColor        = m_shadow.m_shader->GetShaderAttribute("LightColor");
    m_shadow.m_attrLightAmbientColor = m_shadow.m_shader->GetShaderAttribute("LightAmbientColor");
    m_shadow.m_attrLightPosition  = m_nonShadow.m_shader->GetShaderAttribute("LightPosition");
    m_shadow.m_attrLightRange     = m_nonShadow.m_shader->GetShaderAttribute("LightRange");
    m_shadow.m_attrCameraPosition    = m_shadow.m_shader->GetShaderAttribute("CameraPosition");
  }

  return true;
}


void GL4DeferredPointLightRenderer::SetDevice(iDevice *device)
{
  m_device = QueryClass<GL4Device>(device);
}

void GL4DeferredPointLightRenderer::SetScene(ce::iGfxScene *scene)
{
  m_scene = scene;
}

void GL4DeferredPointLightRenderer::Render(const Camera *camera,
                                           const Projector *projector,
                                           const GBuffer *gBuffer,
                                           const GL4PointLight *light,
                                           iRenderTarget2D *target)
{
  LightRenderShader &lrs = m_nonShadow;
  if (light->IsCastShadow())
  {
    m_shadowRenderer.SetDevice(m_device);
    m_shadowRenderer.SetScene(m_scene);
    m_shadowRenderer.SetDepthBuffer(gBuffer->GetDepth());
    m_shadowRenderer.SetShadowMap(GetShadowMap());
//    m_shadowRenderer.RenderShadow(light, *camera, *projector);
    lrs = m_shadow;
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

  m_device->SetShader(lrs.m_shader);
  if (lrs.m_attrDiffuseRoughness)
  {
    eTextureUnit unit = m_device->BindTexture(gBuffer->GetDiffuseRoughness());
    lrs.m_attrDiffuseRoughness->Bind(unit);
  }
  if (lrs.m_attrDepth)
  {
    eTextureUnit unit = m_device->BindTexture(gBuffer->GetDepth());
    lrs.m_attrDepth->Bind(unit);
  }
  if (lrs.m_attrShadowMap)
  {
    eTextureUnit unit = m_device->BindTexture(GetShadowMap()->GetColorTexture(0));
    lrs.m_attrShadowMap->Bind(unit);
  }

  if (lrs.m_attrNormal)
  {
    eTextureUnit unit = m_device->BindTexture(gBuffer->GetNormal());
    lrs.m_attrNormal->Bind(unit);
  }
  if (lrs.m_attrLightPosition)
  {
    lrs.m_attrLightPosition->Bind(light->GetPosition());
  }
  if (lrs.m_attrLightRange)
  {
    lrs.m_attrLightRange->Bind(light->GetRange());
  }
  if (lrs.m_attrLightColor)
  {
    lrs.m_attrLightColor->Bind(light->GetColor());
  }
  if (lrs.m_attrLightAmbientColor)
  {
    lrs.m_attrLightAmbientColor->Bind(Color4f());
  }
  if (lrs.m_attrCameraPosition)
  {
    lrs.m_attrCameraPosition->Bind(camera->GetEye());
  }

  m_device->BindMatrices();
  m_device->RenderFullscreen();

}

GL4RenderTarget2D *GL4DeferredPointLightRenderer::GetShadowMap()
{

  if (m_shadowRenderer.IsShadowMapValid(m_shadowMap))
  {
    return m_shadowMap;
  }

  GL4RenderTarget2D *target = m_shadowRenderer.CreateShadowMap();
  CE_SET(m_shadowMap, target);
  return m_shadowMap;
}

}