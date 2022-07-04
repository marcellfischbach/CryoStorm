
#include <ceCore/entity/terrainmeshstate.hh>
#include <ceCore/entity//world.hh>
#include <ceCore/graphics/material/imaterial.hh>
#include <ceCore/graphics/iterrainmesh.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
namespace ce
{

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

void TerrainMeshState::SetMaterial(iMaterial* material)
{
  CE_SET(m_material, material);
  if (m_gfxMesh)
  {
    m_gfxMesh->SetMaterial(m_material);
  }
}

iMaterial* TerrainMeshState::GetMaterial()
{
  return m_material;
}

const iMaterial* TerrainMeshState::GetMaterial() const
{
  return m_material;
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
  if (m_terrainMesh && m_reference)
  {
    m_terrainMesh->SetReferencePoint(m_reference->GetGlobalMatrix().GetTranslation());
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

} // ce