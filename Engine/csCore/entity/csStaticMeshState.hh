

#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/entity/csSpatialState.hh>
#include <csCore/csRef.hh>


namespace cs
{

class csMesh;
class csGfxMesh;
class csSkeleton;
struct iMaterial;

CS_CLASS(jclass="org.cryo.core.entity.CsStaticMeshState")
class CS_CORE_API csStaticMeshState : public CS_SUPER(csSpatialState)
{
CS_CLASS_GEN ;
public:
  csStaticMeshState(const std::string& name = std::string(""));
  ~csStaticMeshState() override;

  CS_FUNCTION()
  virtual void SetMesh(cs::csMesh* mesh);
  CS_NODISCARD const csMesh* GetMesh() const;
  CS_FUNCTION()
  CS_NODISCARD cs::csMesh* GetMesh();

  CS_FUNCTION()
  void SetMaterial(size_t idx, cs::iMaterial *material);
  CS_NODISCARD const cs::iMaterial* GetMaterial(size_t idx) const;
  CS_FUNCTION()
  CS_NODISCARD cs::iMaterial* GetMaterial(size_t idx);

  CS_FUNCTION()
  void SetReceiveShadow (bool receiveShadow);
  CS_FUNCTION()
  CS_NODISCARD bool IsReceiveShadow () const;

  CS_FUNCTION()
  void SetCastShadow (bool castShadow);
  CS_FUNCTION()
  CS_NODISCARD bool IsCastShadow () const;

  /**
   * @name Livecycle
  * @{
  */
  void OnAttachedToWorld(csWorld* world) override;
  void OnDetachedFromWorld(csWorld* world) override;

  /**
   * @}
   */

protected:
  virtual csGfxMesh* CreateGfxMesh();
  void TransformationUpdatedPreChildren() override;

private:
  struct GfxMeshData
  {
    csRef<csGfxMesh> gfxMesh;
    size_t materialSlot;
  };

  void AddMeshToScene(csWorld* world);
  void RemoveMeshFromScene(csWorld* world);

  void Clear();

  csAssetRef<csMesh> m_mesh;
  bool m_receiveShadow = true;
  bool                    m_castShadow = true;
  std::vector<GfxMeshData>           m_gfxMeshes;
  std::vector<csAssetRef<iMaterial>> m_materials;
};


}