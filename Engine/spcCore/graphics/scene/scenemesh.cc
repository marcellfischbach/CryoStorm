
#include <spcCore/graphics/scene/scenemesh.hh>
#include <spcCore/graphics/material/imaterial.hh>
#include <spcCore/graphics/irendermesh.hh>


namespace spc
{

SceneMesh::SceneMesh()
  : iObject()
  , m_material(nullptr)
  , m_mesh(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
}


SceneMesh::~SceneMesh()
{
  SPC_RELEASE(m_material);
  SPC_RELEASE(m_mesh);
}

void SceneMesh::Render(iDevice* device, eRenderPass pass)
{
  if (m_material && m_mesh)
  {
    if (m_material->Bind(device, pass))
    {
      m_mesh->Render(device, pass);
    }
  }
}

void SceneMesh::SetMaterial(iMaterial* material)
{
  SPC_SET(m_material, material);
}

iMaterial* SceneMesh::GetMaterial()
{
  return m_material;
}

const iMaterial* SceneMesh::GetMaterial() const
{
  return m_material;
}


void SceneMesh::SetMesh(iRenderMesh* mesh)
{
  SPC_SET(m_mesh, mesh);
}

iRenderMesh* SceneMesh::GetMesh()
{
  return m_mesh;
}

const iRenderMesh* SceneMesh::GetMesh() const
{
  return m_mesh;
}



}