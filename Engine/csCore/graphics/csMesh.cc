

#include <csCore/graphics/csMesh.hh>
#include <csCore/csOutOfBoundsException.hh>


namespace cs
{

csMesh::csMesh()
{

}

csMesh::~csMesh()
{

}

const csBoundingBox & csMesh::GetBoundingBox() const
{
  return m_boundingBox;
}


Size csMesh::AddMaterialSlot(const std::string& name, iMaterial *defaultMaterial)
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

void csMesh::SetDefaultMaterial(Size idx, iMaterial *defaultMaterial)
{
  if (idx >= m_materialSlots.size())
  {
    throw csOutOfBoundsException();
  }
  m_materialSlots[idx].m_defaultMaterial =  defaultMaterial;
}

Size csMesh::GetNumberOfSubMeshes() const
{
  return m_subMeshes.size();
}

size_t csMesh::AddSubMesh(iRenderMesh* mesh, Size materialSlotIdx)
{
  SubMesh* subMesh = CreateSubMesh();
  subMesh->SetMesh(mesh);
  subMesh->SetMaterialSlotIdx(materialSlotIdx);
  m_subMeshes.push_back(subMesh);
  m_boundingBox.Add(mesh->GetBoundingBox());
  return m_subMeshes.size()-1;
}

csMesh::SubMesh* csMesh::CreateSubMesh()
{
  return new csMesh::SubMesh();
}

csMesh::SubMesh &csMesh::GetSubMesh(Size idx)
{
  if (idx >= m_subMeshes.size())
  {
    throw csOutOfBoundsException();
  }
  return *m_subMeshes[idx];
}

const csMesh::SubMesh &csMesh::GetSubMesh(Size idx) const
{
  if (idx >= m_subMeshes.size())
  {
    throw csOutOfBoundsException();
  }
  return *m_subMeshes[idx];
}

Size csMesh::GetNumberOfMaterialSlots() const
{
  return m_materialSlots.size();
}

const csMesh::MaterialSlot& csMesh::GetMaterialSlot(Size idx) const
{
  if (idx >= m_materialSlots.size())
  {
    throw csOutOfBoundsException();
  }

  return m_materialSlots[idx];
}

size_t csMesh::IndexOfMaterialSlot(const std::string &materialName) const
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
 *      csMesh::SubMesh
 * 
 * *********************************************************************
 * *********************************************************************/



csMesh::SubMesh::SubMesh()
  : m_mesh(nullptr)
  , m_materialSlotIdx(0)
{

}



csMesh::SubMesh::SubMesh(const SubMesh& mesh)
  : m_mesh(nullptr)
  , m_materialSlotIdx(mesh.m_materialSlotIdx)
{
  m_mesh = mesh.m_mesh;
}


csMesh::SubMesh::~SubMesh()
{
}

csMesh::SubMesh &csMesh::SubMesh::operator=(const SubMesh & mesh)
{
  m_mesh = mesh.m_mesh;
  m_materialSlotIdx = mesh.m_materialSlotIdx;
  return *this;
}




void csMesh::SubMesh::SetMesh(iRenderMesh* mesh)
{
  m_mesh = mesh;
}

iRenderMesh* csMesh::SubMesh::GetMesh() const
{
  return m_mesh;
}

void csMesh::SubMesh::SetMaterialSlotIdx(Size materialSlotIdx)
{
  m_materialSlotIdx = materialSlotIdx;
}

Size csMesh::SubMesh::GetMaterialSlotIdx() const
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


csMesh::MaterialSlot::MaterialSlot(const std::string& name, iMaterial *defaultMaterial)
  : m_defaultMaterial(nullptr)
  , m_name(name)
{
  m_defaultMaterial = defaultMaterial;
}

csMesh::MaterialSlot::MaterialSlot(const MaterialSlot& slot)
: m_defaultMaterial(nullptr)
{
  m_defaultMaterial =  slot.m_defaultMaterial;
  m_name = slot.m_name;
}

csMesh::MaterialSlot::~MaterialSlot()
{

}

csMesh::MaterialSlot& csMesh::MaterialSlot::operator=(const MaterialSlot& slot)
{
  m_defaultMaterial =  slot.m_defaultMaterial;
  m_name = slot.m_name;
  return *this;
}

iMaterial *csMesh::MaterialSlot::GetDefaultMaterial() const
{
  return m_defaultMaterial;
}

const std::string& csMesh::MaterialSlot::GetName() const
{
  return m_name;
}

}