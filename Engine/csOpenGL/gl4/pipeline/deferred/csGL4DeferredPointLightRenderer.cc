
#include <csOpenGL/gl4/pipeline/deferred/csGL4DeferredPointLightRenderer.hh>
#include <csOpenGL/gl4/csGL4Device.hh>
#include <csOpenGL/gl4/csGL4PointLight.hh>
#include <csOpenGL/gl4/csGL4RenderTarget2D.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/csGBuffer.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/shading/iShaderAttribute.hh>
#include <csCore/math/csMath.hh>

namespace cs::opengl
{


bool csGL4DeferredPointLightRenderer::Initialize()
{
  m_shadowRenderer.Initialize();

  m_nonShadow.m_shader = csAssetManager::Get()->Get<iShader>(
      csResourceLocator("${engine}/opengl/gl4/deferred/point_light_deferred_no_shadow.shader"));
  if (m_nonShadow.m_shader)
  {
    m_nonShadow.m_attrRectMin           = m_nonShadow.m_shader->GetShaderAttribute("RectMin");
    m_nonShadow.m_attrRectMax           = m_nonShadow.m_shader->GetShaderAttribute("RectMax");
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

  m_shadow.m_shader = csAssetManager::Get()->Get<iShader>(
      csResourceLocator("${engine}/opengl/gl4/deferred/point_light_deferred_shadow.shader"));
  if (m_shadow.m_shader)
  {

    m_shadow.m_attrRectMin           = m_nonShadow.m_shader->GetShaderAttribute("RectMin");
    m_shadow.m_attrRectMax           = m_nonShadow.m_shader->GetShaderAttribute("RectMax");
    m_shadow.m_attrDiffuseRoughness  = m_shadow.m_shader->GetShaderAttribute("DiffuseRoughness");
    m_shadow.m_attrNormal            = m_shadow.m_shader->GetShaderAttribute("Normal");
    m_shadow.m_attrDepth             = m_shadow.m_shader->GetShaderAttribute("Depth");
    m_shadow.m_attrShadowMap         = m_shadow.m_shader->GetShaderAttribute("ShadowMap");
    m_shadow.m_attrLightColor        = m_shadow.m_shader->GetShaderAttribute("LightColor");
    m_shadow.m_attrLightAmbientColor = m_shadow.m_shader->GetShaderAttribute("LightAmbientColor");
    m_shadow.m_attrLightPosition     = m_nonShadow.m_shader->GetShaderAttribute("LightPosition");
    m_shadow.m_attrLightRange        = m_nonShadow.m_shader->GetShaderAttribute("LightRange");
    m_shadow.m_attrCameraPosition    = m_shadow.m_shader->GetShaderAttribute("CameraPosition");
  }

  return true;
}


void csGL4DeferredPointLightRenderer::SetDevice(iDevice *device)
{
  m_device = csQueryClass<csGL4Device>(device);
}

void csGL4DeferredPointLightRenderer::SetScene(cs::iGfxScene *scene)
{
  m_scene = scene;
}

void csGL4DeferredPointLightRenderer::Render(const csCamera *camera,
                                             const csProjector *projector,
                                             const csGBuffer *gBuffer,
                                             const csGL4PointLight *light,
                                             iRenderTarget2D *target)
{
  LightRenderShader &lrs = m_nonShadow;
  if (light->IsCastShadow())
  {
    m_shadowRenderer.SetDevice(m_device);
    m_shadowRenderer.SetScene(m_scene);
    m_shadowRenderer.SetDepthBuffer(gBuffer->GetDepth());
    m_shadowRenderer.SetShadowMap(GetShadowMap());
    m_shadowRenderer.RenderShadow(light, *camera, *projector);
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
  const csMatrix4f &viewMatrix       = camera->GetViewMatrix();
  const csMatrix4f &projectionMatrix = projector->GetProjectionMatrix(m_device);

  m_device->SetViewMatrix(viewMatrix, camera->GetViewMatrixInv());
  m_device->SetProjectionMatrix(projectionMatrix, projector->GetProjectionMatrixInv(m_device));

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
    lrs.m_attrLightAmbientColor->Bind(csColor4f());
  }
  if (lrs.m_attrCameraPosition)
  {
    lrs.m_attrCameraPosition->Bind(camera->GetEye());
  }

  m_device->BindMatrices();
  csVector2f rectMin = csVector2f(0.0f, 0.0f);
  csVector2f rectMax = csVector2f(1.0f, 1.0f);
  if ((camera->GetEye() - light->GetPosition()).Length() > light->GetRange() + projector->GetNear())
  {
    CalcSphereSizeOnScreen(viewMatrix, projectionMatrix, light, rectMin, rectMax);
  }
  if (lrs.m_attrRectMin)
  {
    lrs.m_attrRectMin->Bind(rectMin);
  }
  if (lrs.m_attrRectMax)
  {
    lrs.m_attrRectMax->Bind(rectMax);
  }

  m_device->RenderFullscreen();

}

void csGL4DeferredPointLightRenderer::CalcSphereSizeOnScreen(const csMatrix4f &camera,
                                                             const csMatrix4f &projection,
                                                             const csGL4PointLight *light,
                                                             csVector2f &outBottomLeft,
                                                             csVector2f &outTopRight) const
{
#define MIN_MAX min.x = ceMin(min.x, p.x);  min.y = ceMin(min.y, p.y);  max.x = ceMax(max.x, p.x);  max.y = ceMax(max.y, p.y)

  csMatrix4f m   = projection * camera;
  csVector2f min = csVector2f(FLT_MAX, FLT_MAX);
  csVector2f max = csVector2f(-FLT_MAX, -FLT_MAX);
  csVector2f p;

  const csVector3f &lp = light->GetPosition();
  float            lr  = light->GetRange();

  p = OnScreen(lp + csVector3f(-lr, -lr, -lr), m);
  MIN_MAX;
  p = OnScreen(lp + csVector3f(-lr, -lr, lr), m);
  MIN_MAX;
  p = OnScreen(lp + csVector3f(-lr, lr, -lr), m);
  MIN_MAX;
  p = OnScreen(lp + csVector3f(-lr, lr, lr), m);
  MIN_MAX;
  p = OnScreen(lp + csVector3f(lr, -lr, -lr), m);
  MIN_MAX;
  p = OnScreen(lp + csVector3f(lr, -lr, lr), m);
  MIN_MAX;
  p = OnScreen(lp + csVector3f(lr, lr, -lr), m);
  MIN_MAX;
  p = OnScreen(lp + csVector3f(lr, lr, lr), m);
  MIN_MAX;
#undef  MIN_MAX

  // clamp to borders
  min.x = ceMax(min.x, 0.0f);
  min.y = ceMax(min.y, 0.0f);
  max.x = ceMin(max.x, 1.0f);
  max.y = ceMin(max.y, 1.0f);

  outBottomLeft = min;
  outTopRight   = max;

}

csVector2f csGL4DeferredPointLightRenderer::OnScreen(const csVector3f &v, const csMatrix4f &m) const
{
  csVector4f p = m * csVector4f(v, 1.0f);

  return csVector2f((p.x / p.w) * 0.5f + 0.5f,
                  (p.y / p.w) * 0.5f + 0.5f);
}

csGL4RenderTarget2D *csGL4DeferredPointLightRenderer::GetShadowMap()
{

  if (m_shadowRenderer.IsShadowMapValid(m_shadowMap))
  {
    return m_shadowMap;
  }

  csGL4RenderTarget2D *target = m_shadowRenderer.CreateShadowMap();
  CS_SET(m_shadowMap, target);
  return m_shadowMap;
}

}