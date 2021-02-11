
#include <spcCore/entity/staticmeshstate.hh>
#include <spcCore/entity/entity.hh>
#include <spcCore/entity/world.hh>
#include <spcCore/graphics/mesh.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/graphics/scene/gfxmesh.hh>


namespace spc
{


StaticMeshState::StaticMeshState(const std::string& name)
  : SpatialState(name)
  , m_mesh(nullptr)
{

}

StaticMeshState::~StaticMeshState()
{
  SPC_RELEASE(m_mesh);
  for (auto gfxMesh : m_gfxMeshes)
  {
    gfxMesh->Release();
  }
  m_gfxMeshes.clear();
}


void StaticMeshState::SetMesh(Mesh* mesh)
{
  World* world = GetWorld();
  if (m_mesh && world)
  {
    RemoveMeshFromScene(world);
  }
  SPC_SET(m_mesh, mesh);
  if (m_mesh && world)
  {
    AddMeshToScene(world);
  }

}

const Mesh* StaticMeshState::GetMesh() const
{
  return m_mesh;
}

Mesh* StaticMeshState::GetMesh()
{
  return m_mesh;
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
    GfxScene* scene = world->GetScene();
    for (Size i = 0, in = m_mesh->GetNumberOfSubMeshes(); i < in; i++)
    {
      const Mesh::SubMesh& subMesh = m_mesh->GetSubMesh(i);
      const Mesh::MaterialSlot& matSlot = m_mesh->GetMaterialSlot(subMesh.GetMaterialSlotIdx());

      GfxMesh* sceneMesh = new GfxMesh();
      sceneMesh->SetStatic(IsStatic());
      sceneMesh->SetMesh(subMesh.GetMesh());
      sceneMesh->SetMaterial(matSlot.GetDefaultMaterial());
      sceneMesh->SetModelMatrix(GetGlobalMatrix());
      m_gfxMeshes.push_back(sceneMesh);

      scene->Add(sceneMesh);
    }
  }
}


void StaticMeshState::RemoveMeshFromScene(World* world)
{
  if (m_mesh && world)
  {
    GfxScene* scene = world->GetScene();
    for (auto gfxMesh : m_gfxMeshes)
    {
      scene->Remove(gfxMesh);
      gfxMesh->Release();
    }
    m_gfxMeshes.clear();
  }
}

void StaticMeshState::Update(float tpf)
{
  Matrix4f mat = GetGlobalMatrix();
  for (auto gfxMesh : m_gfxMeshes)
  {
    gfxMesh->SetModelMatrix(mat);
  }

}

void StaticMeshState::TransformationUpdated()
{
}

}