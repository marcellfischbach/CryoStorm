#include <ceOpenGL/gl4/pipeline/deferred/gl4deferredpipeline.hh>
#include <ceOpenGL/gl4/pipeline/gl4meshsorter.hh>
#include <ceOpenGL/gl4/gl4directionallight.hh>
#include <ceOpenGL/gl4/gl4pointlight.hh>
#include <ceOpenGL/glerror.hh>
#include <ceCore/settings.hh>
#include <ceCore/engine.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/math/clipper/cameraclipper.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/iskyboxrenderer.hh>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/gbuffer.hh>
#include <ceCore/graphics/postprocessing.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/graphics/scene/gfxcamera.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <algorithm>
#include <GL/glew.h>


namespace ce::opengl
{

const float MinLightInfluence = 0.0f;

GL4DeferredPipeline::GL4DeferredPipeline()
    : iRenderPipeline()
    , m_gBuffer(new GBuffer())
    , m_intermediate(nullptr)
    , m_renderMode(0)
{
  CE_CLASS_GEN_CONSTR;
}

GL4DeferredPipeline::~GL4DeferredPipeline()
{

}


void GL4DeferredPipeline::Initialize()
{
  m_directionalLightRenderer.Initialize();
  m_pointLightRenderer.Initialize();

  m_backMaskShader = AssetManager::Get()->Get<iShader>(
      ResourceLocator("file://${engine}/opengl/gl4/deferred/back_mask.shader"));
  if (m_backMaskShader)
  {
    m_attrBackMaskDepth = m_backMaskShader->GetShaderAttribute("Depth");
  }
}


void GL4DeferredPipeline::Render(iRenderTarget2D *target, const GfxCamera *camera, iDevice *device, iGfxScene *scene)
{
  m_frame++;
  if (SetupVariables(target, camera, device, scene))
  {
    CameraClipper clppr(*m_camera, *m_projector);
    ScanVisibleMeshes(&clppr);
    ScanLightsAndShadows(&clppr);


    RenderGBuffer(target->GetWidth(), target->GetHeight());
    RenderBackground();

    RenderLighting();
    RenderTransparent();
    RenderPostProcessing(target);
    Cleanup();
  }

}

void GL4DeferredPipeline::RenderGBuffer(uint16_t width,
                                        uint16_t height)
{
  BindCamera();

  m_gBuffer->Update(m_device, width, height);
  m_device->SetRenderTarget(m_gBuffer->GetGBuffer());
  m_device->SetRenderBuffer(m_gBuffer->GetBufferIDs());
  m_device->SetDepthTest(true);
  m_device->SetDepthWrite(true);
  m_device->SetColorWrite(true, true, true, true);
  m_device->Clear(true, Color4f(0.0f, 0.0f, 0.0f, 0.0f),
                  m_gfxCamera->GetClearMode() == eClearMode::Depth ||
                  m_gfxCamera->GetClearMode() == eClearMode::DepthColor,
                  1.0f, true, 0);
  m_device->BindMaterial(nullptr, eRP_GBuffer);
  std::vector<GfxMesh *> &meshes = m_collector.GetMeshes(eRenderQueue::Default);


  for (const auto mesh: meshes)
  {
    mesh->Render(m_device, eRP_GBuffer);
  }

}

void GL4DeferredPipeline::RenderBackground()
{
  bool clearColor  =
           (m_gfxCamera->GetClearMode() == eClearMode::Color || m_gfxCamera->GetClearMode() == eClearMode::DepthColor)
           && m_gfxCamera->GetClearColorMode() == eClearColorMode::PlainColor;
  bool clearSkybox =
           (m_gfxCamera->GetClearMode() == eClearMode::Color || m_gfxCamera->GetClearMode() == eClearMode::DepthColor)
           && m_gfxCamera->GetClearColorMode() == eClearColorMode::Skybox;

  if (clearSkybox)
  {
    PrepareSkybox(m_gfxCamera->GetSkyboxRenderer());
  }

  m_device->SetRenderTarget(m_target);
  m_device->SetRenderBuffer(0);
  m_device->SetDepthTest(false);
  m_device->SetDepthWrite(false);
  m_device->SetColorWrite(true, true, true, true);
  m_device->SetBlending(false);
  m_device->Clear(clearColor, m_gfxCamera->GetClearColor(), false, 1.0f, true, 0);

  if (clearColor)
  {
    RenderBackMask();
  }
  else if (clearSkybox)
  {
    RenderSkybox(m_gfxCamera->GetSkyboxRenderer());
  }
}


static bool skyboxPrepared = false;
void GL4DeferredPipeline::PrepareSkybox(iSkyboxRenderer *skyboxRenderer)
{
  if (!skyboxPrepared)
  {
    skyboxRenderer->Render(m_device);
    skyboxPrepared = true;
  }

}

void GL4DeferredPipeline::RenderSkybox(iSkyboxRenderer *skyboxRenderer)
{
  m_skyboxMesh.Render(m_device,
                      (m_projector->GetNear() + m_projector->GetFar()) * 0.5f,
                      skyboxRenderer->GetTexture(),
                      m_gBuffer->GetDepth());
}


void GL4DeferredPipeline::RenderBackMask()
{
  if (!m_backMaskShader || !m_attrBackMaskDepth)
  {
    return;
  }

  m_device->SetShader(m_backMaskShader);
  m_device->ResetTextures();
  eTextureUnit unit = m_device->BindTexture(m_gBuffer->GetDepth());
  m_attrBackMaskDepth->Bind(unit);
  m_device->RenderFullscreen();
}


void GL4DeferredPipeline::ScanLightsAndShadows(iClipper *clipper)
{
  m_globalLights.clear();
  m_staticLights.clear();
  m_dynamicLights.clear();
  m_staticLightsNew.clear();
  size_t lightOffset = 0;
  m_scene->ScanLights(clipper, iGfxScene::eSM_Global,
                      [this, &lightOffset](GfxLight *light) {

                        if (lightOffset >= MaxLights)
                        {
                          return false;
                        }
                        m_renderLights[lightOffset] = light;
                        m_globalLights.push_back(light);
                        lightOffset++;
                        return true;
                      }
  );


  m_scene->ScanLights(clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static, [this](GfxLight *light) -> bool {
    if (light->IsStatic())
    {
      m_staticLights.push_back(light);
      if (light->GetFrame() == 0)
      {
        m_staticLightsNew.push_back(light);
        light->SetFrame(m_frame);
      }
    }
    else
    {
      m_dynamicLights.push_back(light);
    }
    return true;
  });

  for (size_t i = lightOffset; i < MaxLights; i++)
  {
    m_renderLights[i] = nullptr;
  }
  m_numberOfFixedLights = lightOffset;
}

void GL4DeferredPipeline::RenderLighting()
{
  switch (m_renderMode)
  {
    case 0:
      RenderLights();
      break;
    case 2:
      m_device->RenderFullscreen(m_gBuffer->GetDiffuseRoughness());
      break;
    case 1:
      m_device->RenderFullscreen(m_gBuffer->GetNormal());
      break;
    case 3:
      m_device->RenderFullscreen(m_gBuffer->GetDepth());
      break;
  }
}

void GL4DeferredPipeline::RenderLights()
{
  for (const auto &gfxLight: m_globalLights)
  {
    RenderLight(gfxLight);
  }
  for (const auto &gfxLight: m_staticLights)
  {
    RenderLight(gfxLight);
  }
  for (const auto &gfxLight: m_dynamicLights)
  {
    RenderLight(gfxLight);
  }
}

void GL4DeferredPipeline::RenderLight(ce::GfxLight *light)
{
  switch (light->GetLight()->GetType())
  {
    case eLT_Directional:
      RenderDirectionalLight(ce::QueryClass<GL4DirectionalLight>(light->GetLight()));
      break;
    case eLT_Point:
      RenderPointLight(ce::QueryClass<GL4PointLight>(light->GetLight()));
      break;
    default:
      break;
  }
}

void GL4DeferredPipeline::RenderTransparent()
{
  UpdateTransparentTarget();

  m_device->SetRenderTarget(m_transparentTarget);

  BindCamera();


  std::vector<GfxMesh *> &transparentMeshes = m_collector.GetMeshes(eRenderQueue::Transparency);
  for (auto              &mesh: transparentMeshes)
  {
    auto material = mesh->GetMaterial();
    if (material->GetShadingMode() == eShadingMode::Shaded)
    {
      RenderForwardMeshShaded(mesh, m_renderLights, m_numberOfFixedLights);
    }
    else
    {
      RenderForwardMeshUnlit(mesh);
    }
  }
  m_device->SetRenderTarget(nullptr);
}


void GL4DeferredPipeline::RenderForwardMeshShaded(GfxMesh *mesh,
                                                  std::array<const GfxLight *, MaxLights> &lights,
                                                  Size offset)
{
  if (mesh->IsStatic())
  {
    if (mesh->IsLightingDirty())
    {
      mesh->ClearLights();
      AppendLights(mesh, m_staticLights);
      mesh->SetLightingDirty(false);
    }
    else
    {
      AppendLights(mesh, m_staticLightsNew);
    }

    auto dynamicLights = CalcMeshLightInfluences(mesh, m_dynamicLights, true);
    Size numLights     = AssignLights(
        mesh->GetLights(),
        dynamicLights,
        lights,
        offset
    );
    CE_GL_ERROR();
    mesh->RenderForward(m_device, eRP_Forward, lights.data(), numLights);
    CE_GL_ERROR();
  }
  else
  {
    auto staticLights  = CalcMeshLightInfluences(mesh, m_staticLights, true);
    auto dynamicLights = CalcMeshLightInfluences(mesh, m_dynamicLights, true);
    Size numLights     = AssignLights(
        staticLights,
        dynamicLights,
        lights,
        offset
    );
    CE_GL_ERROR();
    mesh->RenderForward(m_device, eRP_Forward, lights.data(), numLights);
    CE_GL_ERROR();
  }


}


size_t GL4DeferredPipeline::AssignLights(
    const std::vector<GfxMesh::Light> &static_lights,
    const std::vector<GfxMesh::Light> &dynamic_lights,
    std::array<const GfxLight *, MaxLights> &lights,
    size_t offset)
{
  for (Size s = 0, d = 0, sn = static_lights.size(), dn = dynamic_lights.size();
       offset < MaxLights && (s < sn || d < dn); offset++)
  {
    if (s < sn && d < dn)
    {
      if (static_lights[s].Influence >= dynamic_lights[d].Influence)
      {
        lights[offset] = static_lights[s++].Light;
      }
      else
      {
        lights[offset] = dynamic_lights[d++].Light;
      }
    }
    else if (s < sn)
    {
      lights[offset] = static_lights[s++].Light;
    }
    else if (d < dn)
    {
      lights[offset] = dynamic_lights[d++].Light;
    }
    else
    {
      break;
    }
  }

  return offset;
}


void GL4DeferredPipeline::AppendLights(GfxMesh *mesh, const std::vector<GfxLight *> &lights) const
{
  if (lights.empty())
  {
    return;
  }

  auto      meshLights = CalcMeshLightInfluences(mesh, lights, false);
  for (auto &meshLight: meshLights)
  {
    mesh->AddLight(meshLight.Light, meshLight.Influence);
  }
  mesh->SortAndLimitLights(MaxLights);
}

void GL4DeferredPipeline::RenderForwardMeshUnlit(GfxMesh *mesh)
{
  mesh->RenderUnlit(m_device, eRP_Forward);
}


float GL4DeferredPipeline::CalcMeshLightInfluence(const GfxLight *light, const GfxMesh *mesh) const
{
  if (!light || !mesh)
  {
    return 0.0f;
  }


  // TODO: Take the power of the light from the light ... currently there is no power in the light
  float lightPower          = 1.0f;
  float lightDistanceFactor = 0.0f;
  switch (light->GetLight()->GetType())
  {
    case eLT_Directional:
      lightDistanceFactor = 1.0f;
      break;
    case eLT_Point:
      auto     pointLight = light->GetLight()->Query<iPointLight>();
      Vector3f lightPos   = pointLight->GetPosition();
      Vector3f meshPos    = mesh->GetModelMatrix().GetTranslation();
      Vector3f delta      = lightPos - meshPos;
      float    halfSize   = mesh->GetMesh()->GetBoundingBox().GetDiagonal() / 2.0f;
      float    distance   = delta.Length();
      float    overlap    = pointLight->GetRange() + halfSize - distance;
      if (overlap > 0.0f)
      {
        lightDistanceFactor = overlap / pointLight->GetRange();
      }
      break;

  }
  return lightPower * lightDistanceFactor;
}

std::vector<GfxMesh::Light> GL4DeferredPipeline::CalcMeshLightInfluences(const GfxMesh *mesh,
                                                                         const std::vector<GfxLight *> &lights,
                                                                         bool sorted
                                                                        ) const
{
  std::vector<GfxMesh::Light> influences;
  for (GfxLight               *light: lights)
  {
    float influence = CalcMeshLightInfluence(light, mesh);
    if (influence <= MinLightInfluence)
    {
      continue;
    }
    GfxMesh::Light l {};
    l.Light     = light;
    l.Influence = influence;
    influences.push_back(l);
  }

  if (sorted)
  {
    std::sort(influences.begin(),
              influences.end(),
              [](GfxMesh::Light &l0, GfxMesh::Light &l1) { return l0.Influence > l1.Influence; }
    );
  }


  return influences;
}


void GL4DeferredPipeline::RenderPostProcessing(iRenderTarget2D *target)
{
  PostProcessing *pp = m_gfxCamera->GetPostProcessing();
  if (pp)
  {
    pp->SetInput(PPImageType::Color, m_target->GetColorTexture(0));
    pp->SetInput(PPImageType::Depth, m_target->GetDepthTexture());
    pp->SetInput(PPImageType::Normal, nullptr);
    pp->Process(m_device, target);
  }

}

void GL4DeferredPipeline::Cleanup()
{
  m_device->BindMaterial(nullptr, eRP_COUNT);

  m_device->SetBlending(false);
  m_device->SetDepthWrite(true);
  m_device->SetDepthTest(true);

  m_device    = nullptr;
  m_gfxCamera = nullptr;
  m_camera    = nullptr;
  m_projector = nullptr;
  m_scene     = nullptr;

}

void GL4DeferredPipeline::RenderDirectionalLight(const GL4DirectionalLight *directionalLight)
{
  m_directionalLightRenderer.Render(m_camera, m_projector, m_gBuffer, directionalLight, m_target);
}

void GL4DeferredPipeline::RenderPointLight(const GL4PointLight *pointLight)
{
  m_pointLightRenderer.Render(m_camera, m_projector, m_gBuffer, pointLight, m_target);
}

bool GL4DeferredPipeline::SetupVariables(iRenderTarget2D *target,
                                         const GfxCamera *camera,
                                         iDevice *device,
                                         iGfxScene *scene)
{
  m_device    = device;
  m_gfxCamera = camera;
  m_camera    = camera->GetCamera();
  m_projector = camera->GetProjector();
  m_scene     = scene;
  m_target    = camera->GetPostProcessing() ? UpdateRenderTarget(device, target) : target;
  m_device->BindMaterial(nullptr, eRP_COUNT);
  m_device->ClearTextureCache();

  UpdateIntermediate();

  m_pointLightRenderer.SetDevice(device);
  m_pointLightRenderer.SetScene(scene);
  m_directionalLightRenderer.SetDevice(device);
  m_directionalLightRenderer.SetScene(scene);


  return m_target;
}

iRenderTarget2D *GL4DeferredPipeline::UpdateRenderTarget(ce::iDevice *device, ce::iRenderTarget2D *target)
{
  if (!target)
  {
    CE_RELEASE(m_target);
    return nullptr;
  }


  iRenderTarget2D *renderTarget = m_target;

  if (renderTarget && renderTarget->GetWidth() == target->GetWidth() &&
      renderTarget->GetHeight() == target->GetHeight())
  {
    // all is properly set up
    return renderTarget;
  }

  //  recreate the target
  CE_RELEASE(renderTarget);

  iRenderTarget2D::Descriptor desc {
      target->GetWidth(),
      target->GetHeight()
  };
  renderTarget = device->CreateRenderTarget(desc);
  if (!renderTarget)
  {
    return nullptr;
  }

  iTexture2D::Descriptor colorDesc {
      ePF_RGBA,
      target->GetWidth(),
      target->GetHeight(),
      false,
      1
  };
  iTexture2D             *colorTexture = device->CreateTexture(colorDesc);
  iSampler               *colorSampler = device->CreateSampler();
  colorSampler->SetFilterMode(eFM_MinMagNearest);
  colorSampler->SetAddressU(eTAM_Clamp);
  colorSampler->SetAddressV(eTAM_Clamp);
  colorSampler->SetAddressW(eTAM_Clamp);
  colorTexture->SetSampler(colorSampler);
  renderTarget->AddColorTexture(colorTexture);
  CE_RELEASE(colorTexture);
  CE_RELEASE(colorSampler);

  if (!renderTarget->Compile())
  {
    CE_RELEASE(renderTarget);
    renderTarget = nullptr;
  }


  return renderTarget;
}

void GL4DeferredPipeline::UpdateIntermediate()
{
  if (!m_intermediate
      || m_intermediate->GetWidth() != m_target->GetWidth()
      || m_intermediate->GetHeight() != m_target->GetHeight())
  {
    CE_RELEASE(m_intermediate);

    iTexture2D::Descriptor colorDesc {};
    colorDesc.Width        = m_target->GetWidth();
    colorDesc.Height       = m_target->GetHeight();
    colorDesc.Format       = ePF_RGBA;
    colorDesc.MipMaps      = false;
    colorDesc.MultiSamples = 1;
    iTexture2D *colorTexture = m_device->CreateTexture(colorDesc);

    iTexture2D::Descriptor depthDesc {};
    depthDesc.Width        = m_target->GetWidth();
    depthDesc.Height       = m_target->GetHeight();
    depthDesc.Format       = ePF_DepthStencil;
    depthDesc.MipMaps      = false;
    depthDesc.MultiSamples = 1;
    iTexture2D *depthTexture = m_device->CreateTexture(depthDesc);

    iRenderTarget2D::Descriptor interDesc {};
    interDesc.Width  = m_target->GetWidth();
    interDesc.Height = m_target->GetHeight();
    m_intermediate = m_device->CreateRenderTarget(interDesc);

    m_intermediate->AddColorTexture(colorTexture);
    m_intermediate->SetDepthTexture(depthTexture);

    colorTexture->Release();
    depthTexture->Release();
  }
}

void GL4DeferredPipeline::UpdateTransparentTarget()
{
  if (!m_transparentTarget
      || m_transparentTarget->GetWidth() != m_target->GetWidth()
      || m_transparentTarget->GetHeight() != m_target->GetHeight())
  {
    CE_RELEASE(m_transparentTarget);

    iRenderTarget2D::Descriptor interDesc {};
    interDesc.Width  = m_target->GetWidth();
    interDesc.Height = m_target->GetHeight();
    m_transparentTarget = m_device->CreateRenderTarget(interDesc);
    m_transparentTarget->AddColorTexture(m_target->GetColorTexture(0));
    m_transparentTarget->SetDepthTexture(m_gBuffer->GetDepth());

  }
}


void GL4DeferredPipeline::BindCamera()
{
  m_camera->Bind(m_device);
  m_projector->Bind(m_device);

}


static bool transparent_mesh_compare_less(const GfxMesh *mesh0, const GfxMesh *mesh1)
{
  return false;
}

void GL4DeferredPipeline::ScanVisibleMeshes(iClipper *clipper)
{
  m_collector.Clear();
  m_scene->ScanMeshes(clipper, m_collector);

  std::vector<GfxMesh *> &defaultMeshes     = m_collector.GetMeshes(eRenderQueue::Default);
  std::vector<GfxMesh *> &transparentMeshes = m_collector.GetMeshes(eRenderQueue::Transparency);


  std::sort(defaultMeshes.begin(), defaultMeshes.end(), material_shader_compare_less_gbuffer);
  std::sort(transparentMeshes.begin(), transparentMeshes.end(), transparent_mesh_compare_less);

}


}



