

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

size_t Mesh::IndexOfMaterialSlot(const std::string &materialName) const
{
  for (size_t i=0; i<m_materialSlots.size(); i++)
  {
    if (m_materialSlots[i].m_name == materialName)
    {
      return i;
    }
  }
  return ~0x00;
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



Mesh::SubMesh::SubMesh(const SubMesh& mesh)
  : m_mesh(nullptr)
  , m_materialSlotIdx(mesh.m_materialSlotIdx)
{
  CE_SET(m_mesh, mesh.m_mesh);
}


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

Mesh::MaterialSlot::MaterialSlot(const MaterialSlot& slot)
: m_defaultMaterial(nullptr)
{
  CE_SET(m_defaultMaterial, slot.m_defaultMaterial);
  m_name = slot.m_name;
}

Mesh::MaterialSlot::~MaterialSlot()
{
  CE_RELEASE(m_defaultMaterial);
  m_defaultMaterial = nullptr;
}

Mesh::MaterialSlot& Mesh::MaterialSlot::operator=(const MaterialSlot& slot)
{
  CE_SET(m_defaultMaterial, slot.m_defaultMaterial);
  m_name = slot.m_name;
  return *this;
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