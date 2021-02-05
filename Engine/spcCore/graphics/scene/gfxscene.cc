
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/graphics/ilight.hh>
#include <spcCore/graphics/scene/gfxscenemesh.hh>


namespace spc
{

GfxScene::GfxScene()
  : iObject()
{
  SPC_CLASS_GEN_CONSTR;
}

void GfxScene::Add(GfxSceneMesh* sceneMesh)
{
  if (sceneMesh)
  {
    if (std::find(m_meshes.begin(), m_meshes.end(), sceneMesh) != m_meshes.end())
    {
      return;
    }
    sceneMesh->AddRef();
    m_meshes.push_back(sceneMesh);
    for (auto light : m_lights)
    {
      sceneMesh->AddLight(light);
    }
  }
}

void GfxScene::Remove(GfxSceneMesh* sceneMesh)
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

void GfxScene::Add(iLight* light)
{
  if (light)
  {
    if (std::find(m_lights.begin(), m_lights.end(), light) != m_lights.end())
    {
      return;
    }
    light->AddRef();
    m_lights.push_back(light);
    for (auto mesh : m_meshes)
    {
      mesh->AddLight(light);
    }
  }
}

void GfxScene::Remove(iLight* light)
{
  if (light)
  {
    auto it = std::find(m_lights.begin(), m_lights.end(), light);
    if (it == m_lights.end())
    {
      return;
    }

    m_lights.erase(it);
    for (auto mesh : m_meshes)
    {
      mesh->RemoveLight(light);
    }
    light->Release();
  }
}

void GfxScene::Render(iDevice* device, eRenderPass pass)
{
  for (auto mesh : m_meshes)
  {
    mesh->Render(device, pass);
  }
}



}