#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>
#include <vector>

namespace ce
{

struct iTerrainMesh;
class GfxMesh;
struct iMaterial;
struct iTexture2D;



CE_CLASS()
class CE_CORE_API TerrainLayerMask : public CE_SUPER(Object)
{
CE_CLASS_GEN;
public:
  TerrainLayerMask() = default;
  ~TerrainLayerMask () override;

  void SetLayerTexture (iTexture2D *layerTexture);
  CE_NODISCARD iTexture2D *GetLayerTexture ();
  CE_NODISCARD const iTexture2D *GetLayerTexture () const;

  void SetMaskTexture (iTexture2D *maskTexture);
  CE_NODISCARD iTexture2D *GetMaskTexture ();
  CE_NODISCARD const iTexture2D *GetMaskTexture () const;

private:
  iTexture2D *m_layerTexture = nullptr;
  iTexture2D *m_maskTexture = nullptr;
};

CE_CLASS()
class CE_CORE_API TerrainLayer : public CE_SUPER(Object)
{
CE_CLASS_GEN;
public:
  TerrainLayer () = default;
  ~TerrainLayer() override;

  void SetDiffuseRoughness (iTexture2D *diffuseRoughness);
  CE_NODISCARD iTexture2D *GetDiffuseRoughness ();
  CE_NODISCARD const iTexture2D *GetDiffuseRoughness () const;

  void SetNormal (iTexture2D *normal);
  CE_NODISCARD iTexture2D *GetNormal ();
  CE_NODISCARD const iTexture2D *GetNormal () const;

private:
  iTexture2D *m_diffuseRoughness = nullptr;
  iTexture2D *m_normal = nullptr;
};


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

  virtual void SetLayerMask (TerrainLayerMask *mask);
  virtual void AddLayer (TerrainLayer *layer) = 0;


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

  void UpdateMaterial();


  bool m_receiveShadow = true;
  bool m_castShadow = true;

  iTerrainMesh *m_terrainMesh = nullptr;
  GfxMesh* m_gfxMesh    = nullptr;
  iMaterial* m_material = nullptr;

  TerrainLayerMask *m_layerMask = nullptr;
  std::vector<TerrainLayer*> m_layers;

  SpatialState *m_reference = nullptr;
};

} // ce

