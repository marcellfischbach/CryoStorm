
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>

namespace ce
{

CE_CLASS()
class CE_CORE_API GfxMeshCompound : public CE_SUPER(GfxMesh)
{
  CE_CLASS_GEN;
public:
  GfxMeshCompound();
  ~GfxMeshCompound() override;

  void AddMesh(GfxMesh* mesh);
  void RemoveMesh(GfxMesh *mesh);

  bool ContainsMesh(const GfxMesh* mesh) const;

  void RegenerateMesh();
private:


  std::vector<GfxMesh*> m_internalMeshes;
};

} // ce
