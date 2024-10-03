

#pragma once


#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>


namespace cryo
{

class Mesh;
class GfxMesh;
class Skeleton;
struct iMaterial;

CS_CLASS()
class CS_CORE_API StaticMeshState : public CS_SUPER(SpatialState)
{
CS_CLASS_GEN ;
public:
  StaticMeshState(const std::string& name = std::string(""));
  ~StaticMeshState() override;


  virtual void SetMesh(Mesh* mesh);
  CS_NODISCARD const Mesh* GetMesh() const;
  CS_NODISCARD Mesh* GetMesh();

  void SetMaterial (Size idx, iMaterial* material);
  CS_NODISCARD const iMaterial *GetMaterial (Size idx) const;
  CS_NODISCARD iMaterial *GetMaterial (Size idx);

  void SetReceiveShadow (bool receiveShadow);
  CS_NODISCARD bool IsReceiveShadow () const;

  void SetCastShadow (bool castShadow);
  CS_NODISCARD bool IsCastShadow () const;

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
  virtual GfxMesh* CreateGfxMesh();
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