
#include <ceCore/entity/terrainmeshstate.hh>
#include <ceCore/entity/camerastate.hh>
#include <ceCore/entity//world.hh>
#include <ceCore/graphics/iterrainmesh.hh>
#include <ceCore/graphics/itexture2d.hh>
#include <ceCore/graphics/material/imaterial.hh>
#include <ceCore/graphics/material/material.hh>
#include <ceCore/graphics/shading/ishader.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/resource/assetmanager.hh>

namespace ce
{



TerrainLayerMask::~TerrainLayerMask()
{
  CE_RELEASE(m_layerTexture);
  CE_RELEASE(m_maskTexture);
}

void TerrainLayerMask::SetLayerTexture(iTexture2D* layerTexture)
{
  CE_SET(m_layerTexture, layerTexture);
}

iTexture2D *TerrainLayerMask::GetLayerTexture()
{
  return m_layerTexture;
}

const iTexture2D *TerrainLayerMask::GetLayerTexture() const
{
  return m_layerTexture;
}


void TerrainLayerMask::SetMaskTexture(iTexture2D* maskTexture)
{
  CE_SET(m_maskTexture, maskTexture);
}

iTexture2D *TerrainLayerMask::GetMaskTexture()
{
  return m_maskTexture;
}

const iTexture2D *TerrainLayerMask::GetMaskTexture() const
{
  return m_maskTexture;
}





TerrainLayer::~TerrainLayer()
{
  CE_RELEASE(m_diffuseRoughness);
  CE_RELEASE(m_normal);
}

void TerrainLayer::SetDiffuseRoughness(iTexture2D* diffuseRoughness)
{
  CE_SET(m_diffuseRoughness, diffuseRoughness);
}

iTexture2D *TerrainLayer::GetDiffuseRoughness()
{
  return m_diffuseRoughness;
}

const iTexture2D *TerrainLayer::GetDiffuseRoughness() const
{
  return m_diffuseRoughness;
}


void TerrainLayer::SetNormal(iTexture2D* normal)
{
  CE_SET(m_normal, normal);
}

iTexture2D *TerrainLayer::GetNormal()
{
  return m_normal;
}

const iTexture2D *TerrainLayer::GetNormal() const
{
  return m_normal;
}




TerrainMeshState::TerrainMeshState()
  : SpatialState()
{
  SetNeedUpdate(true);
}

TerrainMeshState::~TerrainMeshState()
{
  CE_RELEASE(m_terrainMesh);
  CE_RELEASE(m_gfxMesh);
  CE_RELEASE(m_material);
}

void TerrainMeshState::SetTerrainMesh(iTerrainMesh* terrainMesh)
{
  CE_SET(m_terrainMesh, terrainMesh);

  World* world = GetWorld();
  if (m_terrainMesh && world)
  {
    RemoveMeshFromScene(world);
  }
  CE_SET(m_terrainMesh, terrainMesh);
  if (m_terrainMesh && world)
  {
    AddMeshToScene(world);
  }
}

iTerrainMesh* TerrainMeshState::GetTerrainMesh()
{
  return m_terrainMesh;
}

const iTerrainMesh* TerrainMeshState::GetTerrainMesh() const
{
  return m_terrainMesh;
}

void TerrainMeshState::SetLayerMask(TerrainLayerMask* mask)
{
  CE_SET(m_layerMask, mask);
  UpdateMaterial();
}

void TerrainMeshState::AddLayer(TerrainLayer* layer)
{
  auto it = std::find(m_layers.begin(), m_layers.end(), layer);
  if (it != m_layers.end())
  {
    return;
  }

  CE_ADDREF(layer);
  m_layers.emplace_back(layer);

  UpdateMaterial();
}

void TerrainMeshState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
  if (m_gfxMesh)
  {
    m_gfxMesh->SetCastShadow(m_castShadow);
  }
}

bool TerrainMeshState::IsCastShadow() const
{
  return m_castShadow;
}

void TerrainMeshState::SetReceiveShadow(bool receiveShadow)
{
  m_receiveShadow = receiveShadow;
  if (m_gfxMesh)
  {
    m_gfxMesh->SetReceiveShadow(m_receiveShadow);
  }
}

bool TerrainMeshState::IsReceiveShadow() const
{
  return m_receiveShadow;
}

void TerrainMeshState::SetReference(SpatialState* reference)
{
  CE_SET(m_reference, reference);
}

SpatialState *TerrainMeshState::GetReference()
{
  return m_reference;
}

const SpatialState *TerrainMeshState::GetReference() const
{
  return m_reference;
}

void TerrainMeshState::OnAttachedToWorld(World* world)
{
  if (m_terrainMesh)
  {
    AddMeshToScene(world);
  }
}

void TerrainMeshState::OnDetachedFromWorld(World* world)
{
  if (m_terrainMesh)
  {
    RemoveMeshFromScene(world);
  }
}

void TerrainMeshState::Update(float tpf)
{
  const SpatialState *reference = nullptr;
  if (m_reference)
  {
    reference = m_reference;
  }
  else
  {
    World* world = GetWorld();
    if (world)
    {
      reference = world->GetMainCamera();
    }
  }

  if (m_terrainMesh && reference)
  {
    m_terrainMesh->SetReferencePoint(reference->GetGlobalMatrix().GetTranslation());
  }
}

void TerrainMeshState::AddMeshToScene(World* world)
{
  if (m_terrainMesh && m_material && world)
  {
    iGfxScene* scene = world->GetScene();

    m_gfxMesh = new GfxMesh();
    m_gfxMesh->SetStatic(IsStatic());
    m_gfxMesh->SetReceiveShadow(m_receiveShadow);
    m_gfxMesh->SetCastShadow(m_castShadow);;
    m_gfxMesh->SetMesh(m_terrainMesh);
    m_gfxMesh->SetMaterial(m_material);
    m_gfxMesh->SetModelMatrix(GetGlobalMatrix());

    scene->Add(m_gfxMesh);
  }
}

void TerrainMeshState::RemoveMeshFromScene(World* world)
{
  if (m_gfxMesh && world)
  {
    iGfxScene * scene = world->GetScene();
    scene->Remove(m_gfxMesh);
    m_gfxMesh->Release();
  }
}

void TerrainMeshState::TransformationUpdatedPreChildren()
{
  if (m_gfxMesh)
  {
    Matrix4f mat = GetGlobalMatrix();
    m_gfxMesh->SetModelMatrix(mat);
  }
}

void TerrainMeshState::UpdateMaterial()
{
  if (!m_material)
  {
    m_material = AssetManager::Get()->Get<iMaterial>(ResourceLocator("/shaders/terrain/terrain4_material.cef"));
    if (!m_material)
    {
      return;
    }
  }

  for (size_t i=0; i<4; i++)
  {
    if (m_layers.size() > i)
    {
      std::string strDiffuseRoughness = std::string("DiffuseRoughness") + std::to_string(i);
      std::string strNormal           = std::string("Normal") + std::to_string(i);
      TerrainLayer*& layer = m_layers[i];
      m_material->Set(m_material->IndexOf(strDiffuseRoughness), layer->GetDiffuseRoughness());
      m_material->Set(m_material->IndexOf(strNormal), layer->GetNormal());

    }
  }

  if (m_gfxMesh)
  {
    m_gfxMesh->SetMaterial(m_material);
  }

}

} // ce