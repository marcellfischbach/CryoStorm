#pragma once

#include <ceCore/coreexport.hh>
#include <array>
#include <vector>
#include <ceCore/graphics/erenderqueue.hh>

namespace ce
{

class GfxMesh;
class CE_CORE_API  GfxSceneCollector
{

public:
  GfxSceneCollector();
  ~GfxSceneCollector();

  void Clear();

  void AddMesh (GfxMesh* mesh);

  CE_NODISCARD std::vector<GfxMesh*> &GetMeshes(eRenderQueue renderQueue);
  CE_NODISCARD const std::vector<GfxMesh*> &GetMeshes(eRenderQueue renderQueue) const;

private:
  std::array<std::vector<GfxMesh*>, static_cast<size_t>(eRenderQueue::COUNT)> m_meshes;
};


}
