#pragma once

#include <ceCore/coreexport.hh>
#include <array>
#include <vector>
#include <ceCore/graphics/eRenderQueue.hh>

namespace cryo
{

class csGfxMesh;
class CS_CORE_API csGfxSceneCollector
{

public:
  csGfxSceneCollector();
  ~csGfxSceneCollector();

  void Clear();

  void AddMesh(csGfxMesh* mesh);

  CS_NODISCARD std::vector<csGfxMesh*> &GetMeshes(eRenderQueue renderQueue);
  CS_NODISCARD const std::vector<csGfxMesh*> &GetMeshes(eRenderQueue renderQueue) const;

private:
  std::array<std::vector<csGfxMesh*>, static_cast<size_t>(eRenderQueue::COUNT)> m_meshes;
};


}
