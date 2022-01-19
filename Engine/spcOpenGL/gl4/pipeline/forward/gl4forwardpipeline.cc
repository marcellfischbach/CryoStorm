
#include <spcOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>
#include <spcOpenGL/gl4/pipeline/forward/gl4forwardmeshsorter.hh>
#include <spcOpenGL/gl4/gl4device.hh>
#include <spcOpenGL/gl4/gl4directionallight.hh>
#include <spcOpenGL/gl4/gl4pointlight.hh>
#include <spcOpenGL/gl4/gl4rendertargetcube.hh>
#include <spcOpenGL/glerror.hh>
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/projector.hh>
#include <spcCore/graphics/scene/gfxcamera.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/graphics/irendertarget2d.hh>
#include <spcCore/graphics/irendertargetcube.hh>
#include <spcCore/graphics/isampler.hh>
#include <spcCore/math/clipper/boxclipper.hh>
#include <spcCore/math/clipper/multiplaneclipper.hh>
#include <spcCore/math/clipper/sphereclipper.hh>
#include <spcCore/settings.hh>
#include <algorithm>
#include <array>
#include <GL/glew.h>
namespace spc::opengl
{

const float MinLightInfluence = 0.0f;

GL4ForwardPipeline::GL4ForwardPipeline()
    : m_frame(0), m_device(nullptr), m_scene(nullptr), m_target(nullptr)
{
  SPC_CLASS_GEN_CONSTR;

}

void GL4ForwardPipeline::Initialize()
{
  Settings settings(ResourceLocator("file:///config/graphics.spc"));
  m_pointLightRenderer.Initialize(settings);
  m_directionalLightRenderer.Initialize(settings);

}

GL4ForwardPipeline::~GL4ForwardPipeline() noexcept
{

}

bool transparent_mesh_compare_less(const GfxMesh *mesh0, const GfxMesh *mesh1)
{
  return false;
}



void GL4ForwardPipeline::Render(iRenderTarget2D *target,
                                const GfxCamera *camera,
                                iDevice *device,
                                GfxScene *scene
                               )
{
  SPC_GL_ERROR();
  ++m_frame;
  m_device    = device;
  m_camera    = camera->GetCamera();
  m_projector = camera->GetProjector();
  m_scene     = scene;
  m_target    = target;

  m_pointLightRenderer.SetDevice(device);
  m_pointLightRenderer.SetScene(scene);
  m_directionalLightRenderer.SetDevice(device);
  m_directionalLightRenderer.SetScene(scene);


  MultiPlaneClipper clipper(*m_camera, *m_projector);

  m_pointLightRenderer.Clear();
  m_directionalLightRenderer.Clear();
  SPC_GL_ERROR();


  // get all global lights from the scene....
  // global lights are always along the final renderlights
  std::array<const GfxLight *, MaxLights> finalRenderLights      = {};
  Size                                    finalRenderLightOffset = 0;
  scene->ScanLights(&clipper, GfxScene::eSM_Global,
                    [this, &finalRenderLights, &finalRenderLightOffset](GfxLight *light) {

                      if (finalRenderLightOffset >= MaxLights)
                      {
                        return false;
                      }
                      finalRenderLights[finalRenderLightOffset] = light;
                      finalRenderLightOffset++;
                      CollectShadowLights(light);
                      return true;
                    }
  );

  SPC_GL_ERROR();

  //
  // collect the "normal" static and dynamic lights
  m_dynamicLights.clear();
  m_staticLights.clear();
  m_staticLightsNew.clear();
  scene->ScanLights(&clipper, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
                    [this](GfxLight *light) {
                      LightScanned(light);
                      CollectShadowLights(light);
                      return true;
                    }
  );
  SPC_GL_ERROR();

  //
  // Render up to MaxLights shadow maps
  if (camera->IsRenderShadows())
  {
    RenderShadowMaps();
  }

  m_camera->Bind(device);
  m_projector->Bind(device);

  SPC_GL_ERROR();

  //
  // and finally render all visible objects
  m_shadedMeshes.clear();
  m_transparentMeshes.clear();
  m_unshadedMeshes.clear();

  int  countBefore = 0;
  int  countAfter  = 0;
  scene->ScanMeshes(&clipper, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
                    [this /* , &finalRenderLights, &finalRenderLightOffset, &trans*/](GfxMesh *mesh) {
                      auto material = mesh->GetMaterial();
                      if (material->GetRenderQueue() == eRenderQueue::Transparency)
                      {
                        m_transparentMeshes.emplace_back(mesh);
                      }
                      else if (material->GetShadingMode() == eShadingMode::Shaded)
                      {
                        m_shadedMeshes.emplace_back(mesh);
                      }
                      else
                      {
                        m_unshadedMeshes.emplace_back(mesh);
                      }
                    }
  );
  std::sort(m_shadedMeshes.begin(), m_shadedMeshes.end(), material_shader_compare_less_forward);
  std::sort(m_unshadedMeshes.begin(), m_unshadedMeshes.end(), material_shader_compare_less_forward);
  std::sort(m_transparentMeshes.begin(), m_transparentMeshes.end(), transparent_mesh_compare_less);


  device->SetRenderTarget(m_target);
  eClearMode mode = camera->GetClearMode();
  device->Clear(
      mode == eClearMode::Color || mode == eClearMode::DepthColor,
      camera->GetClearColor(),
      mode == eClearMode::Depth || mode == eClearMode::DepthColor,
      camera->GetClearDepth(),
      true,
      0
  );


  for (auto mesh: m_shadedMeshes)
  {
    RenderMesh(mesh, finalRenderLights, finalRenderLightOffset);
  }
  for (auto mesh: m_unshadedMeshes)
  {
    RenderUnlitMesh(mesh);
  }


  for (auto mesh: m_transparentMeshes)
  {
    auto material = mesh->GetMaterial();
    if (material->GetShadingMode() == eShadingMode::Shaded)
    {
      RenderMesh(mesh, finalRenderLights, finalRenderLightOffset);
    }
    else
    {
      RenderUnlitMesh(mesh);
    }
  }
  device->BindMaterial(nullptr, eRP_COUNT);

  device->SetBlending(false);
  device->SetDepthWrite(true);
  device->SetDepthTest(true);

  SPC_GL_ERROR();


  // for debuging purpose
  iTexture2DArray *debugColor = m_directionalLightRenderer.GetColorTexture();
  if (debugColor && false)
  {
    Size size = target->GetWidth() / 3;


    m_device->SetViewport(0, 0, static_cast<uint16_t>(size), static_cast<uint16_t>(size));
    m_device->RenderFullscreen(debugColor, 0);

    /*
    m_device->SetViewport(size, 0, size, size);
    m_device->RenderFullscreen(debugColor, 1);

    m_device->SetViewport(size * 2, 0, size, size);
    m_device->RenderFullscreen(debugColor, 2);
    */


  }


  //scene->Render(device, spc::eRP_Forward);
  m_device    = nullptr;
  m_camera    = nullptr;
  m_projector = nullptr;
  m_scene     = nullptr;
  m_target    = nullptr;

  SPC_GL_ERROR();

}


void GL4ForwardPipeline::LightScanned(GfxLight *light)
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

void GL4ForwardPipeline::RenderUnlitMesh(GfxMesh *mesh)
{
  mesh->RenderUnlit(m_device, eRP_Forward);
}


void GL4ForwardPipeline::RenderMesh(GfxMesh *mesh, std::array<const GfxLight *, MaxLights> &lights, Size offset)
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
    SPC_GL_ERROR();
    mesh->RenderForward(m_device, eRP_Forward, lights.data(), numLights);
    SPC_GL_ERROR();
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
    SPC_GL_ERROR();
    mesh->RenderForward(m_device, eRP_Forward, lights.data(), numLights);
    SPC_GL_ERROR();
  }

  //printf("  RenderUnlitMesh - done\n");
}


void GL4ForwardPipeline::CollectShadowLights(GfxLight *light)
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
    m_pointLightRenderer.Add(static_cast<GL4PointLight *>(light->GetLight()));
    break;

  case eLT_Directional:
    m_directionalLightRenderer.Add(static_cast<GL4DirectionalLight *>(light->GetLight()));
    break;

  default:
    break;
  }
}


void GL4ForwardPipeline::RenderShadowMaps()
{
  m_device->ClearShadowMaps();

  Size i = m_directionalLightRenderer.RenderShadowMaps(MaxLights, *m_camera, *m_projector);
  m_pointLightRenderer.RenderShadowMaps(MaxLights - i);

}


Size GL4ForwardPipeline::AssignLights(
    const std::vector<GfxMesh::Light> &static_lights,
    const std::vector<GfxMesh::Light> &dynamic_lights,
    std::array<const GfxLight *, MaxLights> &lights,
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


float GL4ForwardPipeline::CalcMeshLightInfluence(const GfxLight *light, const GfxMesh *mesh) const
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


std::vector<GfxMesh::Light> GL4ForwardPipeline::CalcMeshLightInfluences(const GfxMesh *mesh,
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
    GfxMesh::Light l{};
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

void GL4ForwardPipeline::AppendLights(GfxMesh *mesh, const std::vector<GfxLight *> &lights) const
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

}