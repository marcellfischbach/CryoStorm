
#include <spcOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>
#include <spcOpenGL/gl4/gl4device.hh>
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/projector.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/math/clipper/boxclipper.hh>
#include <algorithm>


namespace spc::opengl
{

const int MaxLights = 4;
const float MinLightInfluence = 0.0f;

GL4ForwardPipeline::GL4ForwardPipeline()
  : m_frame(0)
{

}

GL4ForwardPipeline::~GL4ForwardPipeline() noexcept
{

}
void GL4ForwardPipeline::Render(Camera& camera, Projector& projector, iDevice* device, GfxScene* scene)
{
  ++m_frame;
  m_device = device;


  camera.Bind(device);
  projector.Bind(device);
  BoxClipper clipper(Vector3f(-1000.0f, -1000.0f, -1000.0f), Vector3f(1000.0f, 1000.0f, 1000.0f));

  std::vector<GfxMesh*> m_transparency;


  // get all global finalRenderLights from the scene
  const GfxLight* finalRenderLights[MaxLights];
  Size finalRenderLightOffset = 0;
  scene->ScanLights(&clipper, GfxScene::eSM_Global,
    [&finalRenderLights, &finalRenderLightOffset](GfxLight* light) {
      if (finalRenderLightOffset >= MaxLights)
      {
        return false;
      }
      finalRenderLights[finalRenderLightOffset++] = light;
      return true;
    });


  m_dynamicLights.clear();
  m_staticLights.clear();
  m_staticLightsNew.clear();
  scene->ScanLights(&clipper, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
    [this](GfxLight* light)
    {
      LightScanned(light);
      return true;
    });


  scene->ScanMeshes(&clipper, GfxScene::eSM_Dynamic | GfxScene::eSM_Static,
    [this, &finalRenderLights, &finalRenderLightOffset](GfxMesh* mesh)
    {
      MeshScanned(mesh, finalRenderLights, finalRenderLightOffset);
    }
  );

  //scene->Render(device, spc::eRP_Forward);

}

void GL4ForwardPipeline::LightScanned(GfxLight* light)
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

void GL4ForwardPipeline::MeshScanned(GfxMesh* mesh, const GfxLight** lights, Size offset)
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
    Size numLights = AssignLights(mesh,
      mesh->GetLights(), 
      dynamicLights,
      lights,
      offset);
    mesh->RenderForward(m_device, eRP_Forward, lights, numLights);
  }
  else
  {
    auto staticLights = CalcMeshLightInfluences(mesh, m_staticLights, true);
    auto dynamicLights = CalcMeshLightInfluences(mesh, m_dynamicLights, true);
    Size numLights = AssignLights(mesh,
      mesh->GetLights(),
      dynamicLights,
      lights,
      offset);
    mesh->RenderForward(m_device, eRP_Forward, lights, numLights);
  }

}

Size GL4ForwardPipeline::AssignLights(
  GfxMesh* mesh,
  const std::vector<GfxMesh::Light>& static_lights,
  const std::vector<GfxMesh::Light>& dynamic_lights,
  const GfxLight** lights,
  Size offset)
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


float GL4ForwardPipeline::CalcMeshLightInfluence(const GfxLight* light, const GfxMesh* mesh) const
{
  if (!light || !mesh)
  {
    return 0.0f;
  }

  float halfSize = mesh->GetMesh()->GetBoundingBox().GetDiagonal() / 2.0f;
  // TODO: Take the power of the light from the light ... currently there is no power in the light
  float lightPower = 1.0f;
  float lightDistanceFactor = 0.0f;
  switch (light->GetLight()->GetType())
  {
  case eLT_Directional:
    lightDistanceFactor = 1.0f;
    break;
  case eLT_Point:
    auto pointLight = light->GetLight()->Query<iPointLight>();
    Vector3f lightPos = pointLight->GetPosition();
    Vector3f meshPos = mesh->GetModelMatrix().GetTranslation();
    Vector3f delta = lightPos - meshPos;
    float distance = delta.Length();
    float overlap = pointLight->GetRange() + halfSize - distance;
    if (overlap > 0.0f)
    {
      lightDistanceFactor = overlap / pointLight->GetRange();
    }
    break;
  }
  return lightPower * lightDistanceFactor;
}



std::vector<GfxMesh::Light> GL4ForwardPipeline::CalcMeshLightInfluences(const GfxMesh* mesh, const std::vector<GfxLight*>& lights, bool sorted) const
{
  std::vector<GfxMesh::Light> influences;
  for (GfxLight* light : lights)
  {
    float influence = CalcMeshLightInfluence(light, mesh);
    if (influence <= MinLightInfluence)
    {
      continue;
    }
    GfxMesh::Light l{};
    l.Light = light;
    l.Influence = influence;
    influences.push_back(l);
  }

  if (sorted)
  {
    std::sort(influences.begin(), influences.end(), [](GfxMesh::Light& l0, GfxMesh::Light& l1) { return l0.Influence > l1.Influence; });
  }


  return influences;
}

void GL4ForwardPipeline::AppendLights(GfxMesh* mesh, const std::vector<GfxLight*>& lights) const
{
  if (lights.empty())
  {
    return;
  }

  auto meshLights = CalcMeshLightInfluences(mesh, lights, false);
  for (auto meshLight : meshLights)
  {
    mesh->AddLight(meshLight.Light, meshLight.Influence);
  }
  mesh->SortAndLimitLights(MaxLights);
}

}