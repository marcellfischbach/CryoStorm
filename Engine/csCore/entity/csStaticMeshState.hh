

#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/entity/csSpatialState.hh>


namespace cryo
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
  void OnAttachedToWorld(csWorld* world) override;
  void OnDetachedFromWorld(csWorld* world) override;

  /**
   * @}
   */

protected:
  virtual csGfxMesh* CreateGfxMesh();
  void TransformationUpdatedPreChildren() override;

private:
  void AddMeshToScene(csWorld* world);
  void RemoveMeshFromScene(csWorld* world);

  void Clear();

  csMesh * m_mesh;
  bool m_receiveShadow = true;
  bool                    m_castShadow = true;
  std::vector<csGfxMesh*> m_gfxMeshes;
  std::vector<iMaterial*> m_materials;
};


}