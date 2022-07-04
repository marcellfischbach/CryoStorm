#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>


namespace ce
{

struct iTerrainMesh;
class GfxMesh;
struct iMaterial;

CE_CLASS()
class CE_CORE_API TerrainMeshState : public CE_SUPER(SpatialState)
{
  CE_CLASS_GEN;

public:
  TerrainMeshState ();
  ~TerrainMeshState() override;


  void SetTerrainMesh (iTerrainMesh *terrainMesh);
  CE_NODISCARD iTerrainMesh *GetTerrainMesh ();
  CE_NODISCARD const iTerrainMesh *GetTerrainMesh () const;

  void SetMaterial (iMaterial *material);
  CE_NODISCARD iMaterial *GetMaterial ();
  CE_NODISCARD const iMaterial *GetMaterial () const;

  void SetReceiveShadow (bool receiveShadow);
  CE_NODISCARD bool IsReceiveShadow () const;

  void SetCastShadow (bool castShadow);
  CE_NODISCARD bool IsCastShadow () const;


  void SetReference (SpatialState *reference);
  CE_NODISCARD SpatialState *GetReference ();
  CE_NODISCARD const SpatialState *GetReference () const;

  /**
   * @name Livecycle
  * @{
  */
  void OnAttachedToWorld(World* world) override;
  void OnDetachedFromWorld(World* world) override;
  void Update(float tpf) override;

  /**
   * @}
   */

protected:
  void TransformationUpdatedPreChildren() override;

private:
  void AddMeshToScene(World* world);
  void RemoveMeshFromScene(World* world);


  bool m_receiveShadow = true;
  bool m_castShadow = true;

  iTerrainMesh *m_terrainMesh = nullptr;
  GfxMesh* m_gfxMesh    = nullptr;
  iMaterial* m_material = nullptr;

  SpatialState *m_reference = nullptr;
};

} // ce

