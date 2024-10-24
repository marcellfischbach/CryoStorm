#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csSpatialState.hh>
#include <vector>

namespace cs
{

struct iTerrainMesh;
class csGfxMesh;
struct iMaterial;
struct iTexture2D;



CS_CLASS(jclass="org.cryo.core.entity.TerrainLayerMask")
class CS_CORE_API csTerrainLayerMask : public CS_SUPER(csObject)
{
CS_CLASS_GEN;
public:
  csTerrainLayerMask() = default;
  ~csTerrainLayerMask() override;

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

CS_CLASS(jclass="org.cryo.core.entity.TerrainLayer")
class CS_CORE_API csTerrainLayer : public CS_SUPER(csObject)
{
CS_CLASS_GEN;
public:
  csTerrainLayer() = default;
  ~csTerrainLayer() override;

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


CS_CLASS(jclass="org.cryo.core.entity.TerrainMeshState")
class CS_CORE_API csTerrainMeshState : public CS_SUPER(csSpatialState)
{
  CS_CLASS_GEN;

public:
  csTerrainMeshState();
  ~csTerrainMeshState() override;


  void SetTerrainMesh (iTerrainMesh *terrainMesh);
  CS_NODISCARD iTerrainMesh *GetTerrainMesh ();
  CS_NODISCARD const iTerrainMesh *GetTerrainMesh () const;

  virtual void SetLayerMask(csTerrainLayerMask *mask);
  virtual void AddLayer(csTerrainLayer *layer);


  void SetReceiveShadow (bool receiveShadow);
  CS_NODISCARD bool IsReceiveShadow () const;

  void SetCastShadow (bool castShadow);
  CS_NODISCARD bool IsCastShadow () const;


  void SetReference(csSpatialState *reference);
  CS_NODISCARD csSpatialState *GetReference();
  CS_NODISCARD const csSpatialState *GetReference() const;

  /**
   * @name Livecycle
  * @{
  */
  void OnAttachedToWorld(csWorld* world) override;
  void OnDetachedFromWorld(csWorld* world) override;
  void Update(float tpf) override;

  /**
   * @}
   */

protected:
  void TransformationUpdatedPreChildren() override;

private:
  void AddMeshToScene(csWorld* world);
  void RemoveMeshFromScene(csWorld* world);

  void UpdateMaterial();


  bool m_receiveShadow = true;
  bool m_castShadow = true;

  iTerrainMesh *m_terrainMesh = nullptr;
  csGfxMesh * m_gfxMesh  = nullptr;
  iMaterial * m_material = nullptr;

  csTerrainLayerMask           *m_layerMask = nullptr;
  std::vector<csTerrainLayer*> m_layers;

  csSpatialState *m_reference = nullptr;
};

} // ce

