
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <spcCore/graphics/scene/gfxscenecollector.hh>
#include <spcCore/graphics/material/imaterial.hh>

namespace spc
{

GfxSceneCollector::GfxSceneCollector()
{

}

GfxSceneCollector::~GfxSceneCollector()
{
  Clear();
}

void GfxSceneCollector::Clear()
{
  for (auto &meshes : m_meshes)
  {
    meshes.clear();
  }
}


void GfxSceneCollector::AddMesh(GfxMesh *mesh)
{
  if (!mesh || !mesh->GetMaterial())
  {
    return;
  }

  m_meshes[static_cast<size_t>(mesh->GetMaterial()->GetRenderQueue())].emplace_back(mesh);
}


std::vector<GfxMesh*> &GfxSceneCollector::GetMeshes(eRenderQueue renderQueue)
{
  return m_meshes[static_cast<size_t>(renderQueue)];
}


const std::vector<GfxMesh*> &GfxSceneCollector::GetMeshes(eRenderQueue renderQueue) const
{
  return m_meshes[static_cast<size_t>(renderQueue)];
}


}