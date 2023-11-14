//
// Created by Marcell on 11.11.2023.
//

#include <ceOpenGL/gl4/pipeline/pointsm/gl4pointsmrenderer.hh>
#include <ceOpenGL/gl4/gl4device.hh>
#include <ceOpenGL/gl4/gl4pointlight.hh>
#include <ceOpenGL/gl4/gl4rendertarget2d.hh>
#include <ceOpenGL/gl4/gl4rendertargetcube.hh>
#include <ceCore/settings.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/shading/ishaderattribute.hh>


namespace ce::opengl
{


void GL4PointSMRenderer::Initialize(ce::Settings &settings)
{
  m_pointLightShadowBufferSize = settings.GetInt("point_light.shadow_map.size", 1024);
  std::string filter = settings.GetText("point_light.shadow_map.filter.mode", "Plain");
  if (filter == std::string("Plain"))
  {
    m_shadowSamplingMode = ShadowSamplingMode::Plain;
  }
  else if (filter == std::string("PCF"))
  {
    m_shadowSamplingMode = ShadowSamplingMode::PCF;
  }
  else if (filter == std::string("VSM"))
  {
    m_shadowSamplingMode = ShadowSamplingMode::VSM;
  }


  m_shadowMappingShader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/forward/point_light_shadow_map.shader"));
  if (m_shadowMappingShader)
  {
    m_attrLightPosition = m_shadowMappingShader->GetShaderAttribute("LightPosition");
    m_attrMappingBias   = m_shadowMappingShader->GetShaderAttribute("MappingBias");
    m_attrShadowBuffer  = m_shadowMappingShader->GetShaderAttribute("ShadowBuffer");
    m_attrDepthBuffer   = m_shadowMappingShader->GetShaderAttribute("DepthBuffer");
  }


  Vector2f distance       = settings.GetVector2f("point_light.shadow_map.filter.distance", Vector2f(1, 25));
  float    radius         = settings.GetFloat("point_light.shadow_map.filter.radius", 10.0f);
  float    samples        = settings.GetFloat("point_light.shadow_map.filter.samples", 25.0f);
  float    sampleDistance = settings.GetFloat("point_light.shadow_map.filter.sampleDistance", 0.1f);
  m_shadowMapFilter.Initialize(Vector2f(distance.x, distance.y - distance.x), radius, samples, sampleDistance);

}

void GL4PointSMRenderer::SetDevice(GL4Device *device)
{
  CE_SET(m_device, device);
}

void GL4PointSMRenderer::SetDepthBuffer(iTexture2D *depthBuffer)
{
  CE_SET(m_depthBuffer, depthBuffer);
  m_pointLightShadowMapWidth  = m_depthBuffer->GetWidth();
  m_pointLightShadowMapHeight = m_depthBuffer->GetHeight();
}

void GL4PointSMRenderer::SetScene(iGfxScene *scene)
{
  CE_SET(m_scene, scene);
}

void GL4PointSMRenderer::SetShadowMap(GL4RenderTarget2D *shadowMap)
{
  CE_SET(m_pointLightShadowMap, shadowMap);
}

GL4RenderTarget2D *GL4PointSMRenderer::GetShadowMap()
{
  return m_pointLightShadowMap;
}

void GL4PointSMRenderer::RenderShadow(const ce::opengl::GL4PointLight *pointLight,
                                      const ce::Camera &camera,
                                      const ce::Projector &projector)
{
  RenderShadowBuffer(pointLight, camera, projector);
  RenderShadowMap(pointLight, camera, projector);
  FilterShadowMap();
}

void GL4PointSMRenderer::RenderShadowBuffer(const GL4PointLight *pointLight,
                                            const Camera &camera,
                                            const Projector &projector)
{

  float    near = pointLight->GetRange() * 0.001f;
  float    far  = pointLight->GetRange();
  Matrix4f projection;
  Matrix4f projectionInv;
  m_device->GetPerspectiveProjection(-near, near, -near, near, near, far, projection);
  m_device->GetPerspectiveProjectionInv(-near, near, -near, near, near, far, projectionInv);

  Vector3f pos = pointLight->GetPosition();
  Matrix4f views[6];
  views[0].SetLookAt(pos, pos + Vector3f(1, 0, 0), Vector3f(0, -1, 0));
  views[1].SetLookAt(pos, pos + Vector3f(-1, 0, 0), Vector3f(0, -1, 0));
  views[2].SetLookAt(pos, pos + Vector3f(0, 1, 0), Vector3f(0, 0, -1));
  views[3].SetLookAt(pos, pos + Vector3f(0, -1, 0), Vector3f(0, 0, 1));
  views[4].SetLookAt(pos, pos + Vector3f(0, 0, -1), Vector3f(0, -1, 0));
  views[5].SetLookAt(pos, pos + Vector3f(0, 0, 1), Vector3f(0, -1, 0));

  Matrix4f viewsInv[6];
  viewsInv[0].SetLookAtInv(pos, pos + Vector3f(1, 0, 0), Vector3f(0, -1, 0));
  viewsInv[1].SetLookAtInv(pos, pos + Vector3f(-1, 0, 0), Vector3f(0, -1, 0));
  viewsInv[2].SetLookAtInv(pos, pos + Vector3f(0, 1, 0), Vector3f(0, 0, -1));
  viewsInv[3].SetLookAtInv(pos, pos + Vector3f(0, -1, 0), Vector3f(0, 0, 1));
  viewsInv[4].SetLookAtInv(pos, pos + Vector3f(0, 0, -1), Vector3f(0, -1, 0));
  viewsInv[5].SetLookAtInv(pos, pos + Vector3f(0, 0, 1), Vector3f(0, -1, 0));

  GL4TextureCube *colorTexture = GetPointLightShadowBufferColor();
  GL4TextureCube *depthTexture = GetPointLightShadowBufferDepth();


  m_device->SetProjectionMatrix(projection);

  glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
  for (int i = 0; i < 6; i++)
  {

    if (colorTexture)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             colorTexture->GetName(),
                             0);
    }
    if (depthTexture)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER,
//GL_DEPTH_STENCIL_ATTACHMENT,
                             GL_DEPTH_ATTACHMENT,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             depthTexture->GetName(),
                             0);
    }

    m_device->SetViewport(0, 0, (uint16_t) m_pointLightShadowBufferSize, (uint16_t) m_pointLightShadowBufferSize);
    m_device->SetDepthWrite(true);
    m_device->SetDepthTest(true);
    m_device->SetColorWrite(false, false, false, false);

    m_device->Clear(false, Color4f(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f, false, 0);

    m_device->SetViewMatrix(views[i]);


    CameraClipper clipper(viewsInv[i], projectionInv, false, true);

    m_scene->ScanMeshes(&clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static,
                        [this](GfxMesh *mesh)
                        {
                          mesh->RenderUnlit(m_device, eRP_Shadow);
                        }
    );
    m_device->SetColorWrite(true, true, true, true);
  }
  m_device->SetRenderTarget(nullptr);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void GL4PointSMRenderer::RenderShadowMap(const GL4PointLight *pointLight,
                                         const Camera &camera,
                                         const Projector &projector)
{
  m_device->ResetTextures();
  GL4RenderTarget2D *target = GetPointLightShadowMapTemp();
  m_device->SetRenderTarget(target);
  m_device->SetRenderBuffer(0);
  m_device->SetDepthWrite(true);
  m_device->SetDepthTest(false);
  m_device->SetColorWrite(true, true, true, true);
  m_device->Clear(true, Color4f(0.0, 0.0f, 0.0f, 0.0f), true, 1.0f, true, 0);

  m_device->SetShader(m_shadowMappingShader);
  camera.Bind(m_device);
  projector.Bind(m_device);

  if (m_attrLightPosition)
  {
    m_attrLightPosition->Bind(pointLight->GetPosition());
  }
  if (m_attrMappingBias)
  {
    m_attrMappingBias->Bind(Vector3f(pointLight->GetRange() * 0.001f,
                                     pointLight->GetRange(),
                                     pointLight->GetShadowMapBias()));
  }

  if (m_attrShadowBuffer)
  {
    eTextureUnit unit = m_device->BindTexture(GetPointLightShadowBufferDepth());
    m_attrShadowBuffer->Bind(unit);
  }
  if (m_attrDepthBuffer)
  {
    eTextureUnit unit = m_device->BindTexture(m_depthBuffer);
    m_attrDepthBuffer->Bind(unit);
  }

  m_device->BindMatrices();
  m_device->BindStandardValues();

  m_device->RenderFullscreen();
}

void GL4PointSMRenderer::FilterShadowMap()
{
  m_device->ResetTextures();
  GL4RenderTarget2D *target = GetPointLightShadowMap();

  m_shadowMapFilter.Render(m_device,
                           m_depthBuffer,
                           GetPointLightShadowMapTemp()->GetColorTexture(0),
                           target
  );

}


}