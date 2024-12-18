
#include <csOpenGL/gl4/pipeline/forward/csGL4ForwardPipeline.hh>
#include <csOpenGL/gl4/pipeline/csGL4MeshSorter.hh>
#include <csOpenGL/gl4/csGL4Device.hh>
#include <csOpenGL/gl4/csGL4DirectionalLight.hh>
#include <csOpenGL/gl4/csGL4PointLight.hh>
#include <csOpenGL/gl4/csGL4RenderTargetCube.hh>
#include <csOpenGL/csGLError.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/scene/csGfxCamera.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/iRenderTargetCube.hh>
#include <csCore/graphics/iSampler.hh>
#include <csCore/graphics/iSkyboxRenderer.hh>
#include <csCore/graphics/csPostProcessing.hh>
#include <csCore/math/clipper/csBoxClipper.hh>
#include <csCore/math/clipper/csCameraClipper.hh>
#include <csCore/math/clipper/csMultiPlaneClipper.hh>
#include <csCore/math/clipper/csSphereClipper.hh>
#include <csCore/csSettings.hh>
#include <csCore/csObjectRegistry.hh>
#include <algorithm>
#include <array>
#include <GL/glew.h>


namespace cs::opengl
{

const float MinLightInfluence = 0.0f;

csGL4ForwardPipeline::csGL4ForwardPipeline()
    : m_frame(0), m_device(nullptr), m_scene(nullptr), m_target(nullptr)
{
  CS_CLASS_GEN_CONSTR;

}

void csGL4ForwardPipeline::Initialize()
{
  m_pointLightRenderer.Initialize();
  m_directionalLightRenderer.Initialize();

}

csGL4ForwardPipeline::~csGL4ForwardPipeline() noexcept
{

}

static bool transparent_mesh_compare_less(const csGfxMesh *mesh0, const csGfxMesh *mesh1)
{
  return false;
}

void csGL4ForwardPipeline::Render(iRenderTarget2D *target,
                                  const csGfxCamera *camera,
                                  iDevice *device,
                                  iGfxScene *scene
                                 )
{
  ++m_frame;
  if (SetupVariables(target, camera, device, scene))
  {


    csCameraClipper clppr(*m_camera, *m_projector);

    ScanVisibleMeshes(&clppr);

    BindCamera();
    RenderDepthToTarget();
    ApplyDepthBufferToLightRenderers();

    CollectLightsAndShadows(&clppr);
    RenderShadowMaps();

    BindCamera();
    RenderBackground();
    RenderOpaqueMeshes();
    RenderTransparentMeshes();
    RenderPostProcessing(target);


    if (csObjectRegistry::Get<csDebugCache>()->IsDebug())
    {
      RenderDebugToTarget();
    }

    Cleanup();
  }

}

bool csGL4ForwardPipeline::SetupVariables(iRenderTarget2D *target,
                                          const csGfxCamera *camera,
                                          iDevice *device,
                                          iGfxScene *scene)
{
  m_device    = device;
  m_gfxCamera = camera;
  m_camera    = camera->GetCamera();
  m_projector = camera->GetProjector();
  m_scene     = scene;
  m_target    = camera->GetPostProcessing() ? UpdateRenderTarget(device, target) : target;

  m_pointLightRenderer.SetDevice(device);
  m_pointLightRenderer.SetScene(scene);
  m_pointLightRenderer.Clear();
  m_directionalLightRenderer.SetDevice(device);
  m_directionalLightRenderer.SetScene(scene);
  m_directionalLightRenderer.Clear();

  return m_target;
}


iRenderTarget2D *csGL4ForwardPipeline::UpdateRenderTarget(cs::iDevice *device, cs::iRenderTarget2D *target)
{
  if (!target)
  {
    CS_RELEASE(m_target);
    return nullptr;
  }


  iRenderTarget2D *renderTarget = m_target;

  if (renderTarget && renderTarget->GetWidth() == target->GetWidth() &&
      renderTarget->GetHeight() == target->GetHeight())
  {
    // all is properly set up
    return renderTarget;
  }

  if (renderTarget)
  {
    printf ("Update render target %dx%d -> %dx%d\n",
            m_target->GetWidth(), m_target->GetHeight(),
            target->GetWidth(), target->GetHeight()
            );
  }
  else
  {
    printf ("Render target is null\n");
  }


  //  recreate the target
  CS_RELEASE(renderTarget);

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
  CS_RELEASE(colorTexture);
  CS_RELEASE(colorSampler);

  iTexture2D::Descriptor depthDesc {
      ePF_Depth,
      target->GetWidth(),
      target->GetHeight(),
      false,
      1
  };
  iTexture2D             *depthTexture = device->CreateTexture(depthDesc);
  iSampler               *depthSampler = device->CreateSampler();
  depthSampler->SetFilterMode(eFM_MinMagNearest);
  depthSampler->SetAddressU(eTAM_Clamp);
  depthSampler->SetAddressV(eTAM_Clamp);
  depthSampler->SetAddressW(eTAM_Clamp);
  depthSampler->SetTextureCompareFunc(eCF_LessOrEqual);
  depthSampler->SetTextureCompareMode(eTCM_None);
  depthTexture->SetSampler(depthSampler);
  renderTarget->SetDepthTexture(depthTexture);
  CS_RELEASE(depthTexture);
  CS_RELEASE(depthSampler);

  if (!renderTarget->Compile())
  {
    printf ("render target not compiled\n");
    CS_RELEASE(renderTarget);
    renderTarget = nullptr;
  }


  return renderTarget;
}

void csGL4ForwardPipeline::CollectLightsAndShadows(iClipper *clipper)
{
  m_dynamicLights.clear();
  m_staticLights.clear();
  m_staticLightsNew.clear();

  size_t lightOffset = 0;
  m_scene->ScanLights(clipper, iGfxScene::eSM_Global,
                      [this, &lightOffset](csGfxLight *light) {

                        if (lightOffset >= MaxLights)
                        {
                          return false;
                        }
                        m_renderLights[lightOffset] = light;
                        lightOffset++;
                        CollectShadowLights(light);
                        return true;
                      }
  );


  //
  // collect the "normal" static and dynamic lights

  m_scene->ScanLights(clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static,
                      [this](csGfxLight *light) {
                        LightScanned(light);
                        CollectShadowLights(light);
                        return true;
                      }
  );

  for (size_t i = lightOffset; i < MaxLights; i++)
  {
    m_renderLights[i] = nullptr;
  }
  m_numberOfFixedLights = lightOffset;
}

void csGL4ForwardPipeline::ScanVisibleMeshes(iClipper *clipper)
{
  m_collector.Clear();
  m_scene->ScanMeshes(clipper, m_collector);

  std::vector<csGfxMesh *> &defaultMeshes     = m_collector.GetMeshes(eRenderQueue::Default);
  std::vector<csGfxMesh *> &transparentMeshes = m_collector.GetMeshes(eRenderQueue::Transparency);


  std::sort(defaultMeshes.begin(), defaultMeshes.end(), material_shader_compare_less_forward);
  std::sort(transparentMeshes.begin(), transparentMeshes.end(), transparent_mesh_compare_less);
}

void csGL4ForwardPipeline::BindCamera()
{
  m_camera->Bind(m_device);
  m_projector->Bind(m_device);

}

void csGL4ForwardPipeline::RenderDepthToTarget()
{
  m_device->SetRenderTarget(m_target);
  m_device->SetRenderBuffer(0);
  eClearMode mode = m_gfxCamera->GetClearMode();
  m_device->SetColorWrite(true, true, true, true);
  m_device->SetDepthWrite(true);
  m_device->Clear(mode == eClearMode::Color || mode == eClearMode::DepthColor,
                  m_gfxCamera->GetClearColor(),
                  mode == eClearMode::Depth || mode == eClearMode::DepthColor,
                  m_gfxCamera->GetClearDepth(),
                  false,
                  0
  );

  m_device->SetColorWrite(false, false, false, false);
  m_device->SetDepthTest(true);
  m_device->SetDepthWrite(true);
  m_device->SetDepthFunc(eCF_LessOrEqual);
  const std::vector<csGfxMesh *> &defaultMeshes = m_collector.GetMeshes(eRenderQueue::Default);
  for (auto                    &mesh: defaultMeshes)
  {
    RenderUnlitDepthMesh(mesh);
  }

}

void csGL4ForwardPipeline::RenderBackground()
{
  bool clearColor  =
           (m_gfxCamera->GetClearMode() == eClearMode::Color || m_gfxCamera->GetClearMode() == eClearMode::DepthColor)
           && m_gfxCamera->GetClearColorMode() == eClearColorMode::PlainColor;
  bool clearSkybox =
           (m_gfxCamera->GetClearMode() == eClearMode::Color || m_gfxCamera->GetClearMode() == eClearMode::DepthColor)
           && m_gfxCamera->GetClearColorMode() == eClearColorMode::Skybox;
  bool clearDepth  =
           (m_gfxCamera->GetClearMode() == eClearMode::Depth || m_gfxCamera->GetClearMode() == eClearMode::DepthColor);


  if (clearSkybox)
  {
    PrepareSkybox(m_gfxCamera->GetSkyboxRenderer());
  }

  m_device->SetRenderTarget(m_target);
  m_device->SetRenderBuffer(0);
  m_device->SetColorWrite(true, true, true, true);
  m_device->SetDepthTest(true);
  m_device->SetDepthWrite(true);
  m_device->SetBlending(false);
  m_device->SetDepthFunc(eCF_LessOrEqual);

  m_device->Clear(clearColor, m_gfxCamera->GetClearColor(), true, 1.0f, clearDepth, 0);

  if (clearSkybox)
  {
    RenderSkybox(m_gfxCamera->GetSkyboxRenderer());
  }
}

static bool  skyboxPrepared = false;
void csGL4ForwardPipeline::PrepareSkybox(iSkyboxRenderer *skyboxRenderer)
{
  if (!skyboxPrepared)
  {
    skyboxRenderer->Render(m_device);
    skyboxPrepared = true;
  }

}

void csGL4ForwardPipeline::RenderSkybox(iSkyboxRenderer *skyboxRenderer)
{
  m_skyboxMesh.Render(m_device,
                      (m_projector->GetNear() + m_projector->GetFar()) * 0.5f,
                      skyboxRenderer->GetTexture(),
                      nullptr);
}



void csGL4ForwardPipeline::RenderOpaqueMeshes()
{

  std::vector<csGfxMesh *> &defaultMeshes = m_collector.GetMeshes(eRenderQueue::Default);
  for (auto              &mesh: defaultMeshes)
  {
    auto material = mesh->GetMaterial();
    if (material->GetShadingMode() == eShadingMode::Shaded)
    {
      RenderMesh(mesh, m_renderLights, m_numberOfFixedLights);
    }
    else
    {
      RenderUnlitForwardMesh(mesh);
    }
  }

}

void csGL4ForwardPipeline::RenderTransparentMeshes()
{

  m_device->SetDepthWrite(false);
  m_device->SetDepthFunc(eCF_LessOrEqual);


  std::vector<csGfxMesh *> &transparentMeshes = m_collector.GetMeshes(eRenderQueue::Transparency);
  for (auto              &mesh: transparentMeshes)
  {
    auto material = mesh->GetMaterial();
    if (material->GetShadingMode() == eShadingMode::Shaded)
    {
      RenderMesh(mesh, m_renderLights, m_numberOfFixedLights);
    }
    else
    {
      RenderUnlitForwardMesh(mesh);
    }
  }
  m_device->SetRenderTarget(nullptr);
}

void csGL4ForwardPipeline::RenderDebugToTarget()
{
  m_device->SetRenderTarget(m_target);
  m_device->SetRenderBuffer(0);
  m_device->SetDepthWrite(false);
  m_device->SetDepthTest(false);
  m_device->SetColorWrite(true, true, true, true);
  m_device->RenderFullscreen(m_pointLightRenderer.GetColorTexture());
}

void csGL4ForwardPipeline::ApplyDepthBufferToLightRenderers()
{
  m_directionalLightRenderer.SetDepthBuffer(m_target->GetDepthTexture());
  m_pointLightRenderer.SetDepthBuffer(m_target->GetDepthTexture());
}

void csGL4ForwardPipeline::RenderPostProcessing(iRenderTarget2D *target)
{
  csPostProcessing *pp = m_gfxCamera->GetPostProcessing();
  if (pp)
  {
    pp->SetInput(ePPImageType::Color, m_target->GetColorTexture(0));
    pp->SetInput(ePPImageType::Depth, m_target->GetDepthTexture());
    pp->SetInput(ePPImageType::Normal, nullptr);
    pp->Process(m_device, target);
  }

}


void csGL4ForwardPipeline::Cleanup()
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

void csGL4ForwardPipeline::LightScanned(csGfxLight *light)
{
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
}

void csGL4ForwardPipeline::RenderUnlitDepthMesh(csGfxMesh *mesh)
{
  mesh->RenderUnlit(m_device, eRP_Depth);
}

void csGL4ForwardPipeline::RenderUnlitForwardMesh(csGfxMesh *mesh)
{
  mesh->RenderUnlit(m_device, eRP_Forward);
}

void csGL4ForwardPipeline::RenderMesh(csGfxMesh *mesh, std::array<const csGfxLight *, MaxLights> &lights, Size offset)
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
    mesh->RenderForward(m_device, eRP_Forward, lights.data(), numLights);
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
    mesh->RenderForward(m_device, eRP_Forward, lights.data(), numLights);
  }


}

void csGL4ForwardPipeline::CollectShadowLights(csGfxLight *light)
{
  if (!light)
  {
    return;
  }
  auto lght = light->GetLight();
  if (!lght->IsCastShadow())
  {
    return;
  }
  switch (lght->GetType())
  {
    case eLT_Point:
      m_pointLightRenderer.Add(static_cast<csGL4PointLight *>(light->GetLight()));
      break;

    case eLT_Directional:
      m_directionalLightRenderer.Add(static_cast<csGL4DirectionalLight *>(light->GetLight()));
      break;

    default:
      break;
  }
}

void csGL4ForwardPipeline::RenderShadowMaps()
{
  if (m_gfxCamera->IsRenderShadows())
  {
    m_device->ClearShadowMaps();

    m_directionalLightRenderer.RenderShadowMaps(*m_camera, *m_projector);
    m_pointLightRenderer.RenderShadowMaps(*m_camera, *m_projector);
  }
}

Size csGL4ForwardPipeline::AssignLights(
    const std::vector<csGfxMesh::Light> &static_lights,
    const std::vector<csGfxMesh::Light> &dynamic_lights,
    std::array<const csGfxLight *, MaxLights> &lights,
    Size offset
                                       )
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

float csGL4ForwardPipeline::CalcMeshLightInfluence(const csGfxLight *light, const csGfxMesh *mesh) const
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
      csVector3f lightPos = pointLight->GetPosition();
      csVector3f meshPos  = mesh->GetModelMatrix().GetTranslation();
      csVector3f delta    = lightPos - meshPos;
      float      halfSize = mesh->GetMesh()->GetBoundingBox().GetDiagonal() / 2.0f;
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

std::vector<csGfxMesh::Light> csGL4ForwardPipeline::CalcMeshLightInfluences(const csGfxMesh *mesh,
                                                                            const std::vector<csGfxLight *> &lights,
                                                                            bool sorted
                                                                           ) const
{
  std::vector<csGfxMesh::Light> influences;
  for (csGfxLight               *light: lights)
  {
    float influence = CalcMeshLightInfluence(light, mesh);
    if (influence <= MinLightInfluence)
    {
      continue;
    }
    csGfxMesh::Light l {};
    l.Light     = light;
    l.Influence = influence;
    influences.push_back(l);
  }

  if (sorted)
  {
    std::sort(influences.begin(),
              influences.end(),
              [](csGfxMesh::Light &l0, csGfxMesh::Light &l1) { return l0.Influence > l1.Influence; }
    );
  }


  return influences;
}

void csGL4ForwardPipeline::AppendLights(csGfxMesh *mesh, const std::vector<csGfxLight *> &lights) const
{
  if (lights.empty())
  {
    return;
  }

  auto meshLights = CalcMeshLightInfluences(mesh, lights, false);
  for (auto &meshLight: meshLights)
  {
    mesh->AddLight(meshLight.Light, meshLight.Influence);
  }
  mesh->SortAndLimitLights(MaxLights);
}

}