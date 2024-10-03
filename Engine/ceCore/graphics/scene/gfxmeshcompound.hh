
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>

namespace ce
{

CS_CLASS()
class CS_CORE_API GfxMeshCompound : public CS_SUPER(GfxMesh)
{
  CS_CLASS_GEN;
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
