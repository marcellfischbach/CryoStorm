

#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>


namespace ce
{

class Mesh;
class GfxMesh;
struct iMaterial;

CE_CLASS()
class CE_CORE_API StaticMeshState : public CE_SUPER(SpatialState)
{
CE_CLASS_GEN_OBJECT;
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
  CE_NODISCARD bool IsReceiveShadow () const;

  void SetCastShadow (bool castShadow);
  CE_NODISCARD bool IsCastShadow () const;

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