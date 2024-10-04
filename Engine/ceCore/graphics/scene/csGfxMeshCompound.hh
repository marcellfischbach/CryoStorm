
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/csClass.hh>
#include <ceCore/graphics/scene/csGfxMesh.hh>

namespace cryo
{

CS_CLASS()
class CS_CORE_API csGfxMeshCompound : public CS_SUPER(csGfxMesh)
{
  CS_CLASS_GEN;
public:
  csGfxMeshCompound();
  ~csGfxMeshCompound() override;

  void AddMesh(csGfxMesh* mesh);
  void RemoveMesh(csGfxMesh *mesh);

  bool ContainsMesh(const csGfxMesh* mesh) const;

  void RegenerateMesh();
private:


  std::vector<csGfxMesh*> m_internalMeshes;
};

} // ce
