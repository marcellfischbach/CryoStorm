

#include <ceCore/graphics/mesh.hh>
#include <ceCore/outofboundsexception.hh>

namespace ce
{

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

const BoundingBox & Mesh::GetBoundingBox() const
{
  return m_boundingBox;
}


Size Mesh::AddMaterialSlot(const std::string& name, iMaterial* defaultMaterial)
{
  for (Size i = 0; i < m_materialSlots.size(); i++)
  {
    if (m_materialSlots[i].GetName() == name)
    {
      return i;
    }
  }

  m_materialSlots.push_back(MaterialSlot(name, defaultMaterial));
  return m_materialSlots.size() - 1;
}

void Mesh::SetDefaultMaterial(Size idx, iMaterial* defaultMaterial)
{
  if (idx >= m_materialSlots.size())
  {
    throw OutOfBoundsException();
  }
  CE_SET(m_materialSlots[idx].m_defaultMaterial, defaultMaterial);
}

Size Mesh::GetNumberOfSubMeshes() const
{
  return m_subMeshes.size();
}

void Mesh::AddSubMesh(iRenderMesh* mesh, Size materialSlotIdx)
{
  SubMesh subMesh;
  subMesh.SetMesh(mesh);
  subMesh.SetMaterialSlotIdx(materialSlotIdx);
  m_subMeshes.push_back(subMesh);
  m_boundingBox.Add(mesh->GetBoundingBox());
}

const Mesh::SubMesh &Mesh::GetSubMesh(Size idx) const
{
  if (idx >= m_subMeshes.size())
  {
    throw OutOfBoundsException();
  }
  return m_subMeshes[idx];
}

Size Mesh::GetNumberOfMaterialSlots() const
{
  return m_materialSlots.size();
}

const Mesh::MaterialSlot& Mesh::GetMaterialSlot(Size idx) const
{
  if (idx >= m_materialSlots.size())
  {
    throw OutOfBoundsException();
  }

  return m_materialSlots[idx];
}




/* *********************************************************************
 * ********************************************************************* 
 * 
 *      SubMesh
 * 
 * *********************************************************************
 * *********************************************************************/



Mesh::SubMesh::SubMesh()
  : m_mesh(nullptr)
  , m_materialSlotIdx(0)
{

}
/*
Mesh::SubMesh::SubMesh(iRenderMesh* mesh, Size materialSlotIdx)
: m_mesh(nullptr)
, m_materialSlotIdx(materialSlotIdx)
{
  CE_SET(m_mesh, mesh);
}
*/


Mesh::SubMesh::SubMesh(const SubMesh& mesh)
  : m_mesh(nullptr)
  , m_materialSlotIdx(mesh.m_materialSlotIdx)
{
  CE_SET(m_mesh, mesh.m_mesh);
}

/*
Mesh::SubMesh::SubMesh(SubMesh&& mesh) noexcept
: m_mesh(mesh.m_mesh)
, m_materialSlotIdx(mesh.m_materialSlotIdx)
{
  mesh.m_mesh = nullptr;
}
*/
Mesh::SubMesh::~SubMesh()
{
  CE_RELEASE(m_mesh);
  m_mesh = nullptr;
}

Mesh::SubMesh &Mesh::SubMesh::operator=(const SubMesh & mesh)
{
  CE_SET(m_mesh, mesh.m_mesh);
  m_materialSlotIdx = mesh.m_materialSlotIdx;
  return *this;
}


/*
Mesh::SubMesh &Mesh::SubMesh::operator=(SubMesh && mesh) noexcept
{
  m_mesh = mesh.m_mesh;
  mesh.m_mesh = nullptr;
  m_materialSlotIdx = mesh.m_materialSlotIdx;
  return *this;
}
*/


void Mesh::SubMesh::SetMesh(iRenderMesh* mesh)
{
  CE_SET(m_mesh, mesh);
}

iRenderMesh* Mesh::SubMesh::GetMesh() const
{
  return m_mesh;
}

void Mesh::SubMesh::SetMaterialSlotIdx(Size materialSlotIdx)
{
  m_materialSlotIdx = materialSlotIdx;
}

Size Mesh::SubMesh::GetMaterialSlotIdx() const
{
  return m_materialSlotIdx;
}

/* *********************************************************************
 * *********************************************************************
 *
 *      MaterialSlot
 *
 * *********************************************************************
 * *********************************************************************/


Mesh::MaterialSlot::MaterialSlot(const std::string& name, iMaterial* defaultMaterial)
  : m_defaultMaterial(nullptr)
  , m_name(name)
{
  CE_SET(m_defaultMaterial, defaultMaterial);
}

Mesh::MaterialSlot::~MaterialSlot()
{
  CE_RELEASE(m_defaultMaterial);
  m_defaultMaterial = nullptr;
}



iMaterial* Mesh::MaterialSlot::GetDefaultMaterial() const
{
  return m_defaultMaterial;
}

const std::string& Mesh::MaterialSlot::GetName() const
{
  return m_name;
}

}