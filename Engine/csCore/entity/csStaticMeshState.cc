
#include <csCore/entity/csStaticMeshState.hh>
#include <csCore/entity/csEntity.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/graphics/scene/csGfxMesh.hh>

namespace cs
{


csStaticMeshState::csStaticMeshState(const std::string &name)
    : csSpatialState(name)
    , m_mesh(nullptr)
{
}

csStaticMeshState::~csStaticMeshState()
{
  Clear();
}

void csStaticMeshState::Clear()
{
}


void csStaticMeshState::SetMesh(csMesh *mesh)
{
  csWorld *world = GetWorld();
  if (m_mesh && world)
  {
    RemoveMeshFromScene(world);
  }
  Clear();
  m_mesh = mesh;
  if (m_mesh)
  {
    m_materials.resize(m_mesh->GetNumberOfMaterialSlots(), nullptr);
  }
  if (m_mesh && world)
  {
    AddMeshToScene(world);
  }

}

const csMesh *csStaticMeshState::GetMesh() const
{
  return m_mesh;
}

csMesh *csStaticMeshState::GetMesh()
{
  return m_mesh;
}

void csStaticMeshState::SetMaterial(Size idx, iMaterial *material)
{
  if (idx < m_materials.size())
  {
    m_materials[idx] = material;

    for (auto &data : m_gfxMeshes)
    {
      if (data.materialSlot == idx)
      {
        data.gfxMesh->SetMaterial(material);
      }
    }
  }
}

const iMaterial *csStaticMeshState::GetMaterial(Size idx) const
{
  return idx < m_materials.size()
         ? m_materials[idx]
         : nullptr;
}

iMaterial *csStaticMeshState::GetMaterial(Size idx)
{
  return idx < m_materials.size()
         ? m_materials[idx]
         : nullptr;
}

void csStaticMeshState::SetReceiveShadow(bool receiveShadow)
{
  m_receiveShadow = receiveShadow;
  for (auto &data: m_gfxMeshes)
  {
    data.gfxMesh->SetReceiveShadow(m_receiveShadow);
  }
}

bool csStaticMeshState::IsReceiveShadow() const
{
  return m_receiveShadow;
}


void csStaticMeshState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
  for (auto &data: m_gfxMeshes)
  {
    data.gfxMesh->SetCastShadow(m_castShadow);
  }
}

bool csStaticMeshState::IsCastShadow() const
{
  return m_castShadow;
}


void csStaticMeshState::OnAttachedToWorld(csWorld *world)
{
  if (m_mesh)
  {
    AddMeshToScene(world);
  }
}


void csStaticMeshState::OnDetachedFromWorld(csWorld *world)
{
  if (m_mesh)
  {
    RemoveMeshFromScene(world);
  }
}

void csStaticMeshState::AddMeshToScene(csWorld *world)
{
  if (m_mesh && world)
  {
    iGfxScene *scene = world->GetScene();
    for (Size i = 0, in = m_mesh->GetNumberOfSubMeshes(); i < in; i++)
    {
      const csMesh::SubMesh &subMesh = m_mesh->GetSubMesh(i);
      size_t                materialSlotIdx = subMesh.GetMaterialSlotIdx();
      csAssetRef<iMaterial> material        = nullptr;
      if (materialSlotIdx < m_materials.size())
      {
        material = m_materials[materialSlotIdx];
      }
      if (!material)
      {
        const csMesh::MaterialSlot &matSlot = m_mesh->GetMaterialSlot(materialSlotIdx);
        material = matSlot.GetDefaultMaterial();
      }

      auto gfxMesh = CreateGfxMesh();
      gfxMesh->SetMesh(subMesh.GetMesh());
      gfxMesh->SetMaterial(material);
      m_gfxMeshes.emplace_back(gfxMesh, materialSlotIdx);

      scene->Add(gfxMesh);
    }
  }
}


csGfxMesh *csStaticMeshState::CreateGfxMesh()
{
  auto gfxMesh = new csGfxMesh();
  gfxMesh->SetBatchable(true);
  gfxMesh->SetStatic(IsStatic());
  gfxMesh->SetReceiveShadow(m_receiveShadow);
  gfxMesh->SetCastShadow(m_castShadow);;
  gfxMesh->SetModelMatrix(GetGlobalMatrix());
  return gfxMesh;
}


void csStaticMeshState::RemoveMeshFromScene(csWorld *world)
{
  if (m_mesh && world)
  {
    iGfxScene *scene = world->GetScene();
    for (auto &data: m_gfxMeshes)
    {
      scene->Remove(data.gfxMesh);
    }
    m_gfxMeshes.clear();
  }
}

void csStaticMeshState::TransformationUpdatedPreChildren()
{
  csMatrix4f mat = GetGlobalMatrix();
  for (auto &data: m_gfxMeshes)
  {
    data.gfxMesh->SetModelMatrix(mat);
  }

}

}