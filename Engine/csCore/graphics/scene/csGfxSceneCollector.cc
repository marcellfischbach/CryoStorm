
#include <csCore/graphics/scene/csGfxMesh.hh>
#include <csCore/graphics/scene/csGfxSceneCollector.hh>
#include <csCore/graphics/material/iMaterial.hh>

namespace cryo
{

csGfxSceneCollector::csGfxSceneCollector()
{

}

csGfxSceneCollector::~csGfxSceneCollector()
{
  Clear();
}

void csGfxSceneCollector::Clear()
{
  for (auto &meshes : m_meshes)
  {
    meshes.clear();
  }
}


void csGfxSceneCollector::AddMesh(csGfxMesh *mesh)
{
  if (!mesh || !mesh->GetMaterial())
  {
    return;
  }

  m_meshes[static_cast<size_t>(mesh->GetMaterial()->GetRenderQueue())].emplace_back(mesh);
}


std::vector<csGfxMesh*> &csGfxSceneCollector::GetMeshes(eRenderQueue renderQueue)
{
  return m_meshes[static_cast<size_t>(renderQueue)];
}


const std::vector<csGfxMesh*> &csGfxSceneCollector::GetMeshes(eRenderQueue renderQueue) const
{
  return m_meshes[static_cast<size_t>(renderQueue)];
}


}