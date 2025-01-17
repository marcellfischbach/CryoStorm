

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

CS_CLASS()
class CS_CORE_API csStaticMeshState : public CS_SUPER(csSpatialState)
{
CS_CLASS_GEN ;
public:
  csStaticMeshState(const std::string& name = std::string(""));
  ~csStaticMeshState() override;


  virtual void SetMesh(csMesh* mesh);
  CS_NODISCARD const csMesh* GetMesh() const;
  CS_NODISCARD csMesh* GetMesh();

  void SetMaterial(Size idx, csAssetRef<iMaterial> &material);
  CS_NODISCARD const csAssetRef<iMaterial> GetMaterial(Size idx) const;
  CS_NODISCARD csAssetRef<iMaterial> GetMaterial(Size idx);

  void SetReceiveShadow (bool receiveShadow);
  CS_NODISCARD bool IsReceiveShadow () const;

  void SetCastShadow (bool castShadow);
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