

#pragma once


#include <spcCore/coreexport.hh>
#include <spcCore/entity/spatialstate.hh>


namespace spc
{

class Mesh;
class GfxMesh;
struct iMaterial;

SPC_CLASS()
class SPC_CORE_API StaticMeshState : public SPC_SUPER(SpatialState)
{
SPC_CLASS_GEN;
public:
  StaticMeshState(const std::string& name = std::string(""));
  virtual ~StaticMeshState();


  void SetMesh(Mesh* mesh);
  const Mesh* GetMesh() const;
  Mesh* GetMesh();

  void SetMaterial (Size idx, iMaterial* material);
  const iMaterial *GetMaterial (Size idx) const;
  iMaterial *GetMaterial (Size idx);

  void SetReceiveShadow (bool receiveShadow);
  SPC_NODISCARD bool IsReceiveShadow () const;

  void SetCastShadow (bool castShadow);
  SPC_NODISCARD bool IsCastShadow () const;

  /**
   * @name Livecycle
  * @{
  */
  void OnAttachedToWorld(World* world) override;
  void OnDetachedFromWorld(World* world) override;

  /**
   * @}
   */

protected:
  void TransformationUpdatedPreChildren() override;

private:
  void AddMeshToScene(World* world);
  void RemoveMeshFromScene(World* world);

  void Clear();

  Mesh* m_mesh;
  bool m_receiveShadow = true;
  bool m_castShadow = true;
  std::vector<GfxMesh*> m_gfxMeshes;
  std::vector<iMaterial*> m_materials;
};


}