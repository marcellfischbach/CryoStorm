
#include <spcCore/graphics/scene/scene.hh>
#include <spcCore/graphics/ilight.hh>
#include <spcCore/graphics/scene/scenemesh.hh>


namespace spc
{

Scene::Scene()
: iObject()
{
  SPC_CLASS_GEN_CONSTR;
}

void Scene::Add(SceneMesh *sceneMesh)
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

void Scene::Add(iLight *light)
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

void Scene::Render(iDevice * device, eRenderPass pass)
{
  for (auto mesh : m_meshes)
  {
    mesh->Render(device, pass);
  }
}



}