#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csSpatialState.hh>
#include <vector>
#include <cscore/csRef.hh>


namespace cs
{

struct iTerrainMesh;
class csGfxMesh;
struct iMaterial;
struct iTexture2D;




CS_CLASS(jclass="org.cryo.core.entity.TerrainLayerMask")
class CS_CORE_API csTerrainLayerMask : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  csTerrainLayerMask() = default;
  ~csTerrainLayerMask() override;

  CS_FUNCTION()
  void SetLayerTexture (cs::iTexture2D *layerTexture);
  CS_FUNCTION()
  CS_NODISCARD cs::iTexture2D *GetLayerTexture ();
  CS_NODISCARD const iTexture2D *GetLayerTexture () const;

  CS_FUNCTION()
  void SetMaskTexture (cs::iTexture2D *maskTexture);
  CS_FUNCTION()
  CS_NODISCARD cs::iTexture2D *GetMaskTexture ();
  CS_NODISCARD const iTexture2D *GetMaskTexture () const;

private:
  iTexture2D *m_layerTexture = nullptr;
  iTexture2D *m_maskTexture = nullptr;
};

CS_CLASS(jclass="org.cryo.core.entity.TerrainLayer")
class CS_CORE_API csTerrainLayer : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  csTerrainLayer() = default;
  ~csTerrainLayer() override;

  CS_FUNCTION()
  void SetDiffuseRoughness (cs::iTexture2D *diffuseRoughness);
  CS_FUNCTION()
  CS_NODISCARD cs::iTexture2D *GetDiffuseRoughness ();
  CS_NODISCARD const iTexture2D *GetDiffuseRoughness () const;

  CS_FUNCTION()
  void SetNormal (cs::iTexture2D *normal);
  CS_FUNCTION()
  CS_NODISCARD cs::iTexture2D *GetNormal ();
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


  CS_FUNCTION()
  void SetTerrainMesh (cs::iTerrainMesh *terrainMesh);
  CS_FUNCTION()
  CS_NODISCARD cs::iTerrainMesh *GetTerrainMesh ();
  CS_NODISCARD const iTerrainMesh *GetTerrainMesh () const;

  CS_FUNCTION()
  virtual void SetLayerMask(cs::csTerrainLayerMask *mask);
  CS_FUNCTION()
  virtual void AddLayer(cs::csTerrainLayer *layer);


  CS_FUNCTION()
  void SetReceiveShadow (bool receiveShadow);
  CS_FUNCTION()
  CS_NODISCARD bool IsReceiveShadow () const;

  CS_FUNCTION()
  void SetCastShadow (bool castShadow);
  CS_FUNCTION()
  CS_NODISCARD bool IsCastShadow () const;


  CS_FUNCTION()
  void SetReference(cs::csSpatialState *reference);
  CS_FUNCTION()
  CS_NODISCARD cs::csSpatialState *GetReference();
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
  csRes<iMaterial> m_material;

  csTerrainLayerMask           *m_layerMask = nullptr;
  std::vector<csTerrainLayer*> m_layers;

  csSpatialState *m_reference = nullptr;
};

} // ce

