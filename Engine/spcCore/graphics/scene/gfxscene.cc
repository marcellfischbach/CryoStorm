
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/graphics/ilight.hh>
#include <spcCore/graphics/ipointlight.hh>
#include <spcCore/graphics/mesh.hh>
#include <algorithm>


namespace spc
{

const int   MaxLights         = 4;
const float MinLightInfluence = 0.0f;

GfxScene::GfxScene()
        : iObject()
{
  SPC_CLASS_GEN_CONSTR;
}

void GfxScene::Add(GfxMesh *mesh)
{
  if (!mesh)
  {
    return;
  }
  if (mesh->GetMaterial() && mesh->GetMaterial()->GetShadingMode() == eShadingMode::Unshaded)
  {
    if (std::ranges::find(m_unshadedMeshes, mesh) != m_unshadedMeshes.end())
    {
      return;
    }
    m_unshadedMeshes.emplace_back(mesh);
  }
  else if (mesh->IsStatic())
  {
    if (std::ranges::find(m_staticMeshes, mesh) != m_staticMeshes.end())
    {
      return;
    }
    m_staticMeshes.emplace_back(mesh);
  }
  else
  {
    if (std::ranges::find(m_dynamicMeshes, mesh) != m_dynamicMeshes.end())
    {
      return;
    }
    m_dynamicMeshes.emplace_back(mesh);
  }

  mesh->AddRef();
  mesh->ClearLights();
  mesh->SetLightingDirty(true);

}

void GfxScene::Remove(GfxMesh *mesh)
{
  if (!mesh)
  {
    return;
  }

  mesh->ClearLights();

  if (!mesh->GetMaterial() || mesh->GetMaterial()->GetShadingMode() == eShadingMode::Unshaded)
  {
    auto it = std::ranges::find(m_unshadedMeshes, mesh);
    if (it != m_unshadedMeshes.end())
    {
      m_unshadedMeshes.erase(it);
      return;
    }
  }
  if (mesh->IsStatic())
  {
    auto it = std::ranges::find(m_staticMeshes, mesh);
    if (it != m_staticMeshes.end())
    {
      m_staticMeshes.erase(it);
      return;
    }
  }
  else
  {
    auto it = std::ranges::find(m_dynamicMeshes, mesh);
    if (it != m_dynamicMeshes.end())
    {
      m_dynamicMeshes.erase(it);
      return;
    }
  }


  mesh->Release();
}

void GfxScene::Add(GfxLight *light)
{
  if (light)
  {
    if (light->GetLight()->GetType() == eLT_Directional)
    {
      Add(light, m_globalLights);
    }
    else if (light->IsStatic())
    {
      Add(light, m_staticLights);
      AddStaticLightToStaticMeshes(light);
    }
    else
    {
      Add(light, m_dynamicLights);
    }
  }
}

void GfxScene::Add(GfxLight *light, std::vector<GfxLight *> &lights)
{
  if (std::find(lights.begin(), lights.end(), light) != lights.end())
  {
    return;
  }
  light->AddRef();
  lights.push_back(light);


}

void GfxScene::Remove(GfxLight *light)
{
  if (light)
  {
    if (light->GetLight()->GetType() == eLT_Directional)
    {
      Remove(light, m_globalLights);
    }
    else if (light->IsStatic())
    {
      Remove(light, m_staticLights);
    }
    else
    {
      Remove(light, m_dynamicLights);
    }
  }
}

void GfxScene::Remove(GfxLight *light, std::vector<GfxLight *> &lights)
{
  auto it = std::find(lights.begin(), lights.end(), light);
  if (it == lights.end())
  {
    return;
  }

  lights.erase(it);
  for (auto mesh: m_staticMeshes)
  {
    mesh->RemoveLight(light);
  }
  light->Release();
}

Size assign_lights(
        GfxMesh */*mesh*/,
        const GfxMesh::Light *static_lights,
        Size num_static,
        const GfxMesh::Light *dynamic_lights,
        Size num_dynamic,
        const GfxLight **lights,
        Size offset,
        Size numberOfLights)
{
  for (Size s = 0, d = 0, sn = num_static, dn = num_dynamic;
       offset < numberOfLights && (s < sn || d < dn); offset++)
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

void GfxScene::Render(iDevice *device, eRenderPass pass)
{
  if (pass == eRP_Forward)
  {
    static const GfxLight *lights[MaxLights];
    Size                  offset = 0;
    for (auto             globalLight: m_globalLights)
    {
      if (offset >= MaxLights)
      {
        break;
      }
      lights[offset++] = globalLight;
    }

    auto      dynamic_lights = new GfxMesh::Light[m_dynamicLights.size()];
    auto      static_lights  = new GfxMesh::Light[m_staticLights.size()];
    for (auto mesh: m_staticMeshes)
    {
      Size numberOfLights = offset;
      if (offset < MaxLights)
      {
        const std::vector<GfxMesh::Light> &stat_lights = mesh->GetLights();
        Size                              numDynamic   = CalcMeshLightInfluences(mesh, m_dynamicLights, dynamic_lights);
        numberOfLights = assign_lights(mesh,
                                       &stat_lights[0],
                                       stat_lights.size(),
                                       dynamic_lights,
                                       numDynamic,
                                       lights,
                                       offset,
                                       MaxLights);
      }
      mesh->RenderForward(device, pass, lights, numberOfLights);
    }
    for (auto mesh: m_dynamicMeshes)
    {
      Size numberOfLights = offset;
      if (offset < MaxLights)
      {
        Size numStatic  = CalcMeshLightInfluences(mesh, m_staticLights, static_lights);
        Size numDynamic = CalcMeshLightInfluences(mesh, m_dynamicLights, dynamic_lights);
        numberOfLights  = assign_lights(mesh,
                                        static_lights,
                                        numStatic,
                                        dynamic_lights,
                                        numDynamic,
                                        lights,
                                        offset,
                                        MaxLights);
      }
      mesh->RenderForward(device, pass, lights, numberOfLights);
    }
  }

}


void GfxScene::AddStaticLightsToMesh(GfxMesh *mesh)
{
  if (mesh->GetNumberOfLights() >= MaxLights)
  {
    return;
  }


  //
  // collect all static lights with the influence on the mesh and sort it by their influence
  std::vector<GfxMesh::Light> influences;
  for (auto                   light: m_staticLights)
  {
    GfxMesh::Light l{};
    l.Light     = light;
    l.Influence = CalcMeshLightInfluence(light, mesh);
    influences.push_back(l);
  }
  std::sort(influences.begin(), influences.end(),
            [](const GfxMesh::Light &i0, const GfxMesh::Light &i1) { return i0.Influence > i1.Influence; });

  //
  // assign the most significant lights to the mesh
  for (auto &inf: influences)
  {
    if (inf.Influence <= MinLightInfluence)
    {
      return;
    }
    mesh->AddLight(inf.Light, inf.Influence);
    if (mesh->GetNumberOfLights() >= MaxLights)
    {
      return;
    }
  }
}

void GfxScene::AddStaticLightToStaticMeshes(GfxLight *light)
{
  for (auto mesh: m_staticMeshes)
  {
    float influence = CalcMeshLightInfluence(light, mesh);
    if (influence <= MinLightInfluence)
    {
      continue;
    }
    if (mesh->GetNumberOfLights() < MaxLights)
    {
      mesh->AddLight(light, influence);
    }
    else
    {

      //
      // find the light with the least influence
      float     leastInfluence = FLT_MAX;
      GfxLight  *leastLight    = nullptr;
      for (auto &assignedLight: mesh->GetLights())
      {
        float assignedInfluence = assignedLight.Influence;
        if (assignedInfluence < leastInfluence)
        {
          leastInfluence = assignedInfluence;
          leastLight     = assignedLight.Light;
        }
      }


      //
      // check if the new light has more influence than the least one and replace them
      if (influence > leastInfluence)
      {
        mesh->RemoveLight(leastLight);
        mesh->AddLight(light, influence);
      }
    }
  }
}

float GfxScene::CalcMeshLightInfluence(GfxLight *light, const GfxMesh *mesh)
{
  if (!light || !mesh)
  {
    return 0.0f;
  }

  float halfSize            = mesh->GetMesh()->GetBoundingBox().GetDiagonal() / 2.0f;
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


Size GfxScene::CalcMeshLightInfluences(const GfxMesh *mesh,
                                       const std::vector<GfxLight *> &lights,
                                       GfxMesh::Light *influences)
{
  Size          num = 0;
  for (GfxLight *light: lights)
  {
    float influence = CalcMeshLightInfluence(light, mesh);
    if (influence <= MinLightInfluence)
    {
      continue;
    }
    influences[num++] = GfxMesh::Light{
            light,
            influence
    };

  }

  std::sort(influences,
            influences + num,
            [](const GfxMesh::Light &i0, const GfxMesh::Light &i1) { return i0.Influence > i1.Influence; });
  return num;
}

void GfxScene::ScanMeshes(const iClipper *clipper,
                          uint32_t scanMask,
                          const std::function<void(GfxMesh *)> &callback) const
{
  if (scanMask & eSM_Static)
  {
    for (auto mesh: m_staticMeshes)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        callback(mesh);
      }
    }
  }

  if (scanMask & eSM_Dynamic)
  {
    for (auto mesh: m_dynamicMeshes)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        callback(mesh);
      }
    }
  }

  if (scanMask & eSM_Unshaded)
  {
    for (auto mesh: m_unshadedMeshes)
    {
      if (!clipper || clipper->Test(mesh->GetBoundingBox()) != eClippingResult::eCR_Outside)
      {
        callback(mesh);
      }
    }
  }

}

void GfxScene::ScanGlobalLights(const std::function<bool(GfxLight *)> &callback) const
{
  for (auto light: m_globalLights)
  {
    if (!callback(light))
    {
      break;
    }
  }
}

void GfxScene::ScanStaticLights(const iClipper *clipper, const std::function<bool(GfxLight *)> &callback) const
{
  for (auto light: m_staticLights)
  {
    const iLight *lght = light->GetLight();
    bool         test  = true;
    if (lght->GetType() == eLT_Point)
    {
      auto *plight = lght->Query<const iPointLight>();
      test = clipper->Test(Sphere(plight->GetPosition(), plight->GetRange())) != eClippingResult::eCR_Outside;
    }
    if (test && !callback(light))
    {
      break;
    }
  }
}

void GfxScene::ScanDynamicLights(const iClipper *clipper, const std::function<bool(GfxLight *)> &callback) const
{
  for (auto light: m_dynamicLights)
  {
    const iLight *lght = light->GetLight();
    bool         test  = true;
    if (lght->GetType() == eLT_Point)
    {
      auto *plight = lght->Query<const iPointLight>();
      test = clipper->Test(Sphere(plight->GetPosition(), plight->GetRange())) != eClippingResult::eCR_Outside;
    }

    if (test && !callback(light))
    {
      break;
    }
  }
}

void GfxScene::ScanLights(const iClipper *clipper,
                          uint32_t scanMask,
                          const std::function<bool(GfxLight *)> &callback) const
{
  if (scanMask & eSM_Global)
  {
    ScanGlobalLights(callback);
  }

  if (scanMask & eSM_Static)
  {
    ScanStaticLights(clipper, callback);
  }

  if (scanMask & eSM_Dynamic)
  {
    ScanDynamicLights(clipper, callback);
  }
}


}