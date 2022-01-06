
#include <spcCore/entity/staticmeshstate.hh>
#include <spcCore/entity/entity.hh>
#include <spcCore/entity/world.hh>
#include <spcCore/graphics/mesh.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/graphics/scene/gfxmesh.hh>


namespace spc
{


StaticMeshState::StaticMeshState(const std::string &name)
    : SpatialState(name), m_mesh(nullptr)
{
}

StaticMeshState::~StaticMeshState()
{
  SPC_RELEASE(m_mesh);
  Clear();
}

void StaticMeshState::Clear()
{
  for (auto gfxMesh: m_gfxMeshes)
  {
    gfxMesh->Release();
  }
  m_gfxMeshes.clear();

  for (auto material: m_materials)
  {
    SPC_RELEASE(material);
  }
  m_materials.clear();
}


void StaticMeshState::SetMesh(Mesh *mesh)
{
  World *world = GetWorld();
  if (m_mesh && world)
  {
    RemoveMeshFromScene(world);
  }
  Clear();
  SPC_SET(m_mesh, mesh);
  if (m_mesh)
  {
    m_materials.resize(m_mesh->GetNumberOfMaterialSlots(), nullptr);
  }
  if (m_mesh && world)
  {
    AddMeshToScene(world);
  }

}

const Mesh *StaticMeshState::GetMesh() const
{
  return m_mesh;
}

Mesh *StaticMeshState::GetMesh()
{
  return m_mesh;
}

void StaticMeshState::SetMaterial(Size idx, iMaterial *material)
{
  if (idx < m_materials.size())
  {
    SPC_SET(m_materials[idx], material);
  }
}

const iMaterial *StaticMeshState::GetMaterial(Size idx) const
{
  return idx < m_materials.size()
         ? m_materials[idx]
         : nullptr;
}

iMaterial *StaticMeshState::GetMaterial(Size idx)
{
  return idx < m_materials.size()
         ? m_materials[idx]
         : nullptr;
}

void StaticMeshState::SetReceiveShadow(bool receiveShadow)
{
  m_receiveShadow = receiveShadow;
  for (auto mesh : m_gfxMeshes)
  {
    mesh->SetReceiveShadow(m_receiveShadow);
  }
}

bool StaticMeshState::IsReceiveShadow() const
{
  return m_receiveShadow;
}


void StaticMeshState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
  for (auto mesh : m_gfxMeshes)
  {
    mesh->SetCastShadow(m_castShadow);
  }
}

bool StaticMeshState::IsCastShadow() const
{
  return m_castShadow;
}


void StaticMeshState::OnAttachedToWorld(World *world)
{
  if (m_mesh)
  {
    AddMeshToScene(world);
  }
}


void StaticMeshState::OnDetachedFromWorld(World *world)
{
  if (m_mesh)
  {
    RemoveMeshFromScene(world);
  }
}

void StaticMeshState::AddMeshToScene(World *world)
{
  if (m_mesh && world)
  {
    GfxScene *scene = world->GetScene();
    for (Size i = 0, in = m_mesh->GetNumberOfSubMeshes(); i < in; i++)
    {
      const Mesh::SubMesh &subMesh = m_mesh->GetSubMesh(i);
      Size materialSlotIdx = subMesh.GetMaterialSlotIdx();
      iMaterial *material = nullptr;
      if (materialSlotIdx < m_materials.size())
      {
        material = m_materials[materialSlotIdx];
      }
      if (!material)
      {
        const Mesh::MaterialSlot &matSlot = m_mesh->GetMaterialSlot(materialSlotIdx);
        material                          = matSlot.GetDefaultMaterial();
      }

      GfxMesh *sceneMesh = new GfxMesh();
      sceneMesh->SetStatic(IsStatic());
      sceneMesh->SetReceiveShadow(m_receiveShadow);
      sceneMesh->SetCastShadow(m_castShadow);;
      sceneMesh->SetMesh(subMesh.GetMesh());
      sceneMesh->SetMaterial(material);
      sceneMesh->SetModelMatrix(GetGlobalMatrix());
      m_gfxMeshes.push_back(sceneMesh);

      scene->Add(sceneMesh);
    }
  }
}


void StaticMeshState::RemoveMeshFromScene(World *world)
{
  if (m_mesh && world)
  {
    GfxScene *scene = world->GetScene();
    for (auto gfxMesh: m_gfxMeshes)
    {
      scene->Remove(gfxMesh);
      gfxMesh->Release();
    }
    m_gfxMeshes.clear();
  }
}

void StaticMeshState::TransformationUpdatedPreChildren()
{
  Matrix4f  mat = GetGlobalMatrix();
  for (auto gfxMesh: m_gfxMeshes)
  {
    gfxMesh->SetModelMatrix(mat);
  }

}

}