
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/graphics/ilight.hh>
#include <spcCore/graphics/ipointlight.hh>
#include <spcCore/graphics/mesh.hh>
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <algorithm>


namespace spc
{

const int MaxLights = 4;
const float MinLightInfluence = 0.0f;

GfxScene::GfxScene()
        : iObject()
{
  SPC_CLASS_GEN_CONSTR;
}

void GfxScene::Add(GfxMesh* sceneMesh)
{
  if (sceneMesh)
  {
    if (std::find(m_meshes.begin(), m_meshes.end(), sceneMesh) != m_meshes.end())
    {
      return;
    }
    sceneMesh->AddRef();
    sceneMesh->ClearLights();
    for (auto light : m_globalLights)
    {
      sceneMesh->AddLight(light);
    }

    if (sceneMesh->IsStatic())
    {
      m_staticMeshes.push_back(sceneMesh);
      AddStaticLightsToMesh(sceneMesh);
    }
    else
    {
      m_meshes.push_back(sceneMesh);
    }
  }
}

void GfxScene::Remove(GfxMesh* sceneMesh)
{
  if (sceneMesh)
  {
    auto it = std::find(m_meshes.begin(), m_meshes.end(), sceneMesh);
    if (it == m_meshes.end())
    {
      return;
    }

    sceneMesh->ClearLights();
    m_meshes.erase(it);
    sceneMesh->Release();
  }
}

void GfxScene::Add(GfxLight* light)
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
      AddStaticLightToMeshes(light);
    }
    else
    {
      Add(light, m_dynamicLights);
    }
  }
}

void GfxScene::Add(GfxLight* light, std::vector<GfxLight*>& lights)
{
  if (std::find(lights.begin(), lights.end(), light) != lights.end())
  {
    return;
  }
  light->AddRef();
  lights.push_back(light);


}

void GfxScene::Remove(GfxLight* light)
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

void GfxScene::Remove(GfxLight* light, std::vector<GfxLight*>& lights)
{
  auto it = std::find(lights.begin(), lights.end(), light);
  if (it == lights.end())
  {
    return;
  }

  lights.erase(it);
  for (auto mesh : m_staticMeshes)
  {
    mesh->RemoveLight(light);
  }
  light->Release();
}

void GfxScene::Render(iDevice* device, eRenderPass pass)
{
  for (auto mesh : m_staticMeshes)
  {
    mesh->Render(device, pass);
  }
  for (auto mesh : m_meshes)
  {
    mesh->Render(device, pass);
  }

}


struct LightInfluenceOnMesh
{
  GfxLight* light;
  float influence;
  LightInfluenceOnMesh(GfxLight* light = nullptr, float influence = FLT_MAX) : light(light), influence(influence)
  {}
};

void GfxScene::AddStaticLightsToMesh(GfxMesh* mesh)
{
  if (mesh->GetNumberOfLights() >= MaxLights)
  {
    return;
  }


  //
  // collect all static lights with the influence on the mesh and sort it by their influence
  std::vector<LightInfluenceOnMesh> influences(m_staticLights.size());
  for (auto light : m_staticLights)
  {
    influences.emplace_back(LightInfluenceOnMesh(light, CalcInfluenceOfLightToMesh(light, mesh)));
  }
  std::sort(influences.begin(), influences.end(),
            [](const LightInfluenceOnMesh& i0, const LightInfluenceOnMesh& i1) { return i0.influence > i1.influence; });

  //
  // assign the most significant lights to the mesh
  for (auto inf : influences)
  {
    if (inf.influence <= MinLightInfluence)
    {
      return;
    }
    mesh->AddLight(inf.light);
    if (mesh->GetNumberOfLights() >= MaxLights)
    {
      return;
    }
  }
}

void GfxScene::AddStaticLightToMeshes(GfxLight* light)
{
  for (auto mesh : m_staticMeshes)
  {
    float influence = CalcInfluenceOfLightToMesh(light, mesh);
    if (influence <= MinLightInfluence)
    {
      continue;
    }
    if (mesh->GetNumberOfLights() < MaxLights)
    {
      mesh->AddLight(light);
    }
    else
    {

      //
      // find the light with the least influence
      float leastInfluence = FLT_MAX;
      GfxLight* leastLight = nullptr;
      for (auto assignedLight : mesh->GetLights())
      {
        float assignedInfluence = CalcInfluenceOfLightToMesh(assignedLight, mesh);
        if (assignedInfluence < leastInfluence)
        {
          leastInfluence = assignedInfluence;
          leastLight = assignedLight;
        }
      }


      //
      // check if the new light has more influence than the least one and replace them
      if (influence > leastInfluence)
      {
        mesh->RemoveLight(leastLight);
        mesh->AddLight(light);
      }
    }
  }
}

float GfxScene::CalcInfluenceOfLightToMesh(const GfxLight* light, const GfxMesh* mesh)
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
      float overlap =  pointLight->GetRange() + halfSize - distance;
      if (overlap > 0.0f)
      {
        lightDistanceFactor = overlap / pointLight->GetRange();
      }
      break;
  }
  return lightPower * lightDistanceFactor;
}


}