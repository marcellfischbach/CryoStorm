#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/spatialstate.hh>
#include <vector>

namespace cryo
{

struct iTerrainMesh;
class GfxMesh;
struct iMaterial;
struct iTexture2D;



CS_CLASS()
class CS_CORE_API TerrainLayerMask : public CS_SUPER(Object)
{
CS_CLASS_GEN;
public:
  TerrainLayerMask() = default;
  ~TerrainLayerMask () override;

  void SetLayerTexture (iTexture2D *layerTexture);
  CS_NODISCARD iTexture2D *GetLayerTexture ();
  CS_NODISCARD const iTexture2D *GetLayerTexture () const;

  void SetMaskTexture (iTexture2D *maskTexture);
  CS_NODISCARD iTexture2D *GetMaskTexture ();
  CS_NODISCARD const iTexture2D *GetMaskTexture () const;

private:
  iTexture2D *m_layerTexture = nullptr;
  iTexture2D *m_maskTexture = nullptr;
};

CS_CLASS()
class CS_CORE_API TerrainLayer : public CS_SUPER(Object)
{
CS_CLASS_GEN;
public:
  TerrainLayer () = default;
  ~TerrainLayer() override;

  void SetDiffuseRoughness (iTexture2D *diffuseRoughness);
  CS_NODISCARD iTexture2D *GetDiffuseRoughness ();
  CS_NODISCARD const iTexture2D *GetDiffuseRoughness () const;

  void SetNormal (iTexture2D *normal);
  CS_NODISCARD iTexture2D *GetNormal ();
  CS_NODISCARD const iTexture2D *GetNormal () const;

private:
  iTexture2D *m_diffuseRoughness = nullptr;
  iTexture2D *m_normal = nullptr;
};


CS_CLASS()
class CS_CORE_API TerrainMeshState : public CS_SUPER(SpatialState)
{
  CS_CLASS_GEN;

public:
  TerrainMeshState ();
  ~TerrainMeshState() override;


  void SetTerrainMesh (iTerrainMesh *terrainMesh);
  CS_NODISCARD iTerrainMesh *GetTerrainMesh ();
  CS_NODISCARD const iTerrainMesh *GetTerrainMesh () const;

  virtual void SetLayerMask (TerrainLayerMask *mask);
  virtual void AddLayer (TerrainLayer *layer);


  void SetReceiveShadow (bool receiveShadow);
  CS_NODISCARD bool IsReceiveShadow () const;

  void SetCastShadow (bool castShadow);
  CS_NODISCARD bool IsCastShadow () const;


  void SetReference (SpatialState *reference);
  CS_NODISCARD SpatialState *GetReference ();
  CS_NODISCARD const SpatialState *GetReference () const;

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

