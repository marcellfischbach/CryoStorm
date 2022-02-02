#pragma once

#include <spcCore/coreexport.hh>
#include <array>
#include <vector>
#include <spcCore/graphics/erenderqueue.hh>

namespace spc
{

class GfxMesh;
class SPC_CORE_API  GfxSceneCollector
{

public:
  GfxSceneCollector();
  ~GfxSceneCollector();

  void Clear();

  void AddMesh (GfxMesh* mesh);

  SPC_NODISCARD std::vector<GfxMesh*> &GetMeshes(eRenderQueue renderQueue);
  SPC_NODISCARD const std::vector<GfxMesh*> &GetMeshes(eRenderQueue renderQueue) const;

private:
  std::array<std::vector<GfxMesh*>, static_cast<size_t>(eRenderQueue::COUNT)> m_meshes;
};


}
