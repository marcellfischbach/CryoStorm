#include <csOpenGL/gl4/pipeline/deferred/csGL4DeferredPipeline.hh>
#include <csOpenGL/gl4/pipeline/csGL4MeshSorter.hh>
#include <csOpenGL/gl4/csGL4DirectionalLight.hh>
#include <csOpenGL/gl4/csGL4PointLight.hh>
#include <csOpenGL/csGLError.hh>
#include <csCore/csSettings.hh>
#include <csCore/csEngine.hh>
#include <csCore/math/iclipper.hh>
#include <csCore/math/clipper/csCameraClipper.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/iSkyboxRenderer.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/csGBuffer.hh>
#include <csCore/graphics/csPostProcessing.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/graphics/scene/csGfxCamera.hh>
#include <csCore/graphics/scene/csGfxMesh.hh>
#include <algorithm>
#include <GL/glew.h>


namespace cs::opengl
{

const float MinLightInfluence = 0.0f;

csGL4DeferredPipeline::csGL4DeferredPipeline()
    : iRenderPipeline()
    , m_gBuffer(new csGBuffer())
    , m_intermediate(nullptr)
    , m_renderMode(0)
{

}

csGL4DeferredPipeline::~csGL4DeferredPipeline()
{

}


void csGL4DeferredPipeline::Initialize()
{
  m_directionalLightRenderer.Initialize();
  m_pointLightRenderer.Initialize();

  m_backMaskShader = csAssetManager::Get()->Get<iShader>(
      csAssetLocator("${engine}/opengl/gl4/deferred/back_mask.shader"));
  if (m_backMaskShader)
  {
    m_attrBackMaskDepth = m_backMaskShader->GetShaderAttribute("Depth");
  }
}


void csGL4DeferredPipeline::Render(iRenderTarget2D *target, const csGfxCamera *camera, iDevice *device, iGfxScene *scene)
{
  m_frame++;
  if (SetupVariables(target, camera, device, scene))
  {
    csCameraClipper clppr(*m_camera, *m_projector);
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

void csGL4DeferredPipeline::RenderGBuffer(uint16_t width,
                                          uint16_t height)
{
  BindCamera();

  m_gBuffer->Update(m_device, width, height);
  m_device->SetRenderTarget(m_gBuffer->GetGBuffer());
  m_device->SetRenderBuffer(m_gBuffer->GetBufferIDs());
  m_device->SetDepthTest(true);
  m_device->SetDepthWrite(true);
  m_device->SetColorWrite(true, true, true, true);
  m_device->Clear(true, csColor4f(0.0f, 0.0f, 0.0f, 0.0f),
                  m_gfxCamera->GetClearMode() == eClearMode::Depth ||
                  m_gfxCamera->GetClearMode() == eClearMode::DepthColor,
                  1.0f, true, 0);
  csAssetRef<iMaterial> null;
  m_device->BindMaterial(null, eRP_GBuffer);
  std::vector<csGfxMesh *> &meshes = m_collector.GetMeshes(eRenderQueue::Default);


  for (const auto mesh: meshes)
  {
    mesh->Render(m_device, eRP_GBuffer);
  }

}

void csGL4DeferredPipeline::RenderBackground()
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
void csGL4DeferredPipeline::PrepareSkybox(iSkyboxRenderer *skyboxRenderer)
{
  if (!skyboxPrepared)
  {
    skyboxRenderer->Render(m_device);
    skyboxPrepared = true;
  }

}

void csGL4DeferredPipeline::RenderSkybox(iSkyboxRenderer *skyboxRenderer)
{
  m_skyboxMesh.Render(m_device,
                      (m_projector->GetNear() + m_projector->GetFar()) * 0.5f,
                      skyboxRenderer->GetTexture(),
                      m_gBuffer->GetDepth());
}


void csGL4DeferredPipeline::RenderBackMask()
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


void csGL4DeferredPipeline::ScanLightsAndShadows(iClipper *clipper)
{
  m_globalLights.clear();
  m_staticLights.clear();
  m_dynamicLights.clear();
  m_staticLightsNew.clear();
  size_t lightOffset = 0;
  m_scene->ScanLights(clipper, iGfxScene::eSM_Global,
                      [this, &lightOffset](csGfxLight *light) {

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


  m_scene->ScanLights(clipper, iGfxScene::eSM_Dynamic | iGfxScene::eSM_Static, [this](csGfxLight *light) -> bool {
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

void csGL4DeferredPipeline::RenderLighting()
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

void csGL4DeferredPipeline::RenderLights()
{
  if (m_gfxCamera->IsRenderShadows())
  {
    m_device->ClearShadowMaps();
    m_directionalLightRenderer.ResetShadowBuffer();
  }
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

void csGL4DeferredPipeline::RenderLight(cs::csGfxLight *light)
{
  switch (light->GetLight()->GetType())
  {
    case eLT_Directional:
      RenderDirectionalLight(cs::csQueryClass<csGL4DirectionalLight>(light->GetLight()));
      break;
    case eLT_Point:
      RenderPointLight(cs::csQueryClass<csGL4PointLight>(light->GetLight()));
      break;
    default:
      break;
  }
}

void csGL4DeferredPipeline::RenderTransparent()
{
  UpdateTransparentTarget();

  m_device->SetRenderTarget(m_transparentTarget);

  BindCamera();


  std::vector<csGfxMesh *> &transparentMeshes = m_collector.GetMeshes(eRenderQueue::Transparency);
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


void csGL4DeferredPipeline::RenderForwardMeshShaded(csGfxMesh *mesh,
                                                    std::array<const csGfxLight *, MaxLights> &lights,
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


size_t csGL4DeferredPipeline::AssignLights(
    const std::vector<csGfxMesh::Light> &static_lights,
    const std::vector<csGfxMesh::Light> &dynamic_lights,
    std::array<const csGfxLight *, MaxLights> &lights,
    size_t offset)
{
  for (Size s = 0, d = 0, sn = static_lights.size(), dn = dynamic_lights.size();
       offset < MaxLights && (s < sn || d < dn); offset++)
  {
    if (s < sn && d < dn)
    {
      if (static_lights[s].influence >= dynamic_lights[d].influence)
      {
        lights[offset] = static_lights[s++].light;
      }
      else
      {
        lights[offset] = dynamic_lights[d++].light;
      }
    }
    else if (s < sn)
    {
      lights[offset] = static_lights[s++].light;
    }
    else if (d < dn)
    {
      lights[offset] = dynamic_lights[d++].light;
    }
    else
    {
      break;
    }
  }

  return offset;
}


void csGL4DeferredPipeline::AppendLights(csGfxMesh *mesh, const std::vector<csGfxLight *> &lights) const
{
  if (lights.empty())
  {
    return;
  }

  auto      meshLights = CalcMeshLightInfluences(mesh, lights, false);
  for (auto &meshLight: meshLights)
  {
    mesh->AddLight(meshLight.light, meshLight.influence);
  }
  mesh->SortAndLimitLights(MaxLights);
}

void csGL4DeferredPipeline::RenderForwardMeshUnlit(csGfxMesh *mesh)
{
  mesh->RenderUnlit(m_device, eRP_Forward);
}


float csGL4DeferredPipeline::CalcMeshLightInfluence(const csGfxLight *light, const csGfxMesh *mesh) const
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

std::vector<csGfxMesh::Light> csGL4DeferredPipeline::CalcMeshLightInfluences(const csGfxMesh *mesh,
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
    influences.emplace_back(light, influence);
  }

  if (sorted)
  {
    std::sort(influences.begin(),
              influences.end(),
              [](csGfxMesh::Light &l0, csGfxMesh::Light &l1) { return l0.influence > l1.influence; }
    );
  }


  return influences;
}


void csGL4DeferredPipeline::RenderPostProcessing(iRenderTarget2D *target)
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

void csGL4DeferredPipeline::Cleanup()
{
  csAssetRef<iMaterial> null;
  m_device->BindMaterial(null, eRP_COUNT);

  m_device->SetBlending(false);
  m_device->SetDepthWrite(true);
  m_device->SetDepthTest(true);

  m_device    = nullptr;
  m_gfxCamera = nullptr;
  m_camera    = nullptr;
  m_projector = nullptr;
  m_scene     = nullptr;

}

void csGL4DeferredPipeline::RenderDirectionalLight(csGL4DirectionalLight *directionalLight)
{
  m_directionalLightRenderer.Render(m_camera, m_projector, m_gBuffer, directionalLight, m_target);
}

void csGL4DeferredPipeline::RenderPointLight(const csGL4PointLight *pointLight)
{
  m_pointLightRenderer.Render(m_camera, m_projector, m_gBuffer, pointLight, m_target);
}

bool csGL4DeferredPipeline::SetupVariables(iRenderTarget2D *target,
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
  csAssetRef<iMaterial> null;
  m_device->BindMaterial(null, eRP_COUNT);
  m_device->ClearTextureCache();

  UpdateIntermediate();

  m_pointLightRenderer.SetDevice(device);
  m_pointLightRenderer.SetScene(scene);
  m_directionalLightRenderer.SetDevice(device);
  m_directionalLightRenderer.SetScene(scene);


  return m_target;
}

iRenderTarget2D *csGL4DeferredPipeline::UpdateRenderTarget(cs::iDevice *device, cs::iRenderTarget2D *target)
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

  //  recreate the target
  CS_RELEASE(renderTarget);

  iRenderTarget2D::Descriptor desc {
      target->GetWidth(),
      target->GetHeight()
  };
  renderTarget = device->CreateRenderTarget(desc).Consume();
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
  csRef<iTexture2D> colorTexture = device->CreateTexture(colorDesc);
  csRef<iSampler> colorSampler = device->CreateSampler();
  colorSampler->SetFilterMode(eFM_MinMagNearest);
  colorSampler->SetAddressU(eTAM_Clamp);
  colorSampler->SetAddressV(eTAM_Clamp);
  colorSampler->SetAddressW(eTAM_Clamp);
  colorTexture->SetSampler(colorSampler);
  renderTarget->AddColorTexture(colorTexture);

  if (!renderTarget->Compile())
  {
    CS_RELEASE(renderTarget);
    renderTarget = nullptr;
  }


  return renderTarget;
}

void csGL4DeferredPipeline::UpdateIntermediate()
{
  if (!m_intermediate
      || m_intermediate->GetWidth() != m_target->GetWidth()
      || m_intermediate->GetHeight() != m_target->GetHeight())
  {
    CS_RELEASE(m_intermediate);

    iTexture2D::Descriptor colorDesc {};
    colorDesc.Width        = m_target->GetWidth();
    colorDesc.Height       = m_target->GetHeight();
    colorDesc.Format       = ePF_RGBA;
    colorDesc.MipMaps      = false;
    colorDesc.MultiSamples = 1;
    auto colorTexture = m_device->CreateTexture(colorDesc).Consume();

    iTexture2D::Descriptor depthDesc {};
    depthDesc.Width        = m_target->GetWidth();
    depthDesc.Height       = m_target->GetHeight();
    depthDesc.Format       = ePF_DepthStencil;
    depthDesc.MipMaps      = false;
    depthDesc.MultiSamples = 1;
    auto depthTexture = m_device->CreateTexture(depthDesc).Consume();

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

void csGL4DeferredPipeline::UpdateTransparentTarget()
{
  if (!m_transparentTarget
      || m_transparentTarget->GetWidth() != m_target->GetWidth()
      || m_transparentTarget->GetHeight() != m_target->GetHeight())
  {
    CS_RELEASE(m_transparentTarget);

    iRenderTarget2D::Descriptor interDesc {};
    interDesc.Width  = m_target->GetWidth();
    interDesc.Height = m_target->GetHeight();
    m_transparentTarget = m_device->CreateRenderTarget(interDesc);
    m_transparentTarget->AddColorTexture(m_target->GetColorTexture(0));
    m_transparentTarget->SetDepthTexture(m_gBuffer->GetDepth());

  }
}


void csGL4DeferredPipeline::BindCamera()
{
  m_camera->Bind(m_device);
  m_projector->Bind(m_device);

}


static bool transparent_mesh_compare_less(const csGfxMesh *mesh0, const csGfxMesh *mesh1)
{
  return false;
}

void csGL4DeferredPipeline::ScanVisibleMeshes(iClipper *clipper)
{
  m_collector.Clear();
  m_scene->ScanMeshes(clipper, m_collector);

  std::vector<csGfxMesh *> &defaultMeshes     = m_collector.GetMeshes(eRenderQueue::Default);
  std::vector<csGfxMesh *> &transparentMeshes = m_collector.GetMeshes(eRenderQueue::Transparency);


  std::sort(defaultMeshes.begin(), defaultMeshes.end(), material_shader_compare_less_gbuffer);
  std::sort(transparentMeshes.begin(), transparentMeshes.end(), transparent_mesh_compare_less);

}


}



