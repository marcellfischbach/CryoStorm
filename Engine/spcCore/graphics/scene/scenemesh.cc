
#include <spcCore/graphics/scene/scenemesh.hh>
#include <spcCore/graphics/material/imaterial.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/idevice.hh>


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
  ClearLights();
}

void SceneMesh::Render(iDevice* device, eRenderPass pass)
{
  if (device->BindMaterial(m_material, pass))
  {
    if (pass == eRP_Forward)
    {
      Size i = 0; 
      for (iLight* light : m_lights)
      {
        device->BindForwardLight(light, i++);
      }
      device->FinishForwardLights(i);
    }

    device->SetModelMatrix(m_modelMatrix);
    device->Render(m_mesh, pass);
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


void SceneMesh::SetModelMatrix(const Matrix4f& modelMatrix)
{
  m_modelMatrix = modelMatrix;
}

const Matrix4f& SceneMesh::GetModelMatrix()  const
{
  return m_modelMatrix;
}

void SceneMesh::ClearLights()
{
  for (iLight* light : m_lights)
  {
    light->Release();
  }
  m_lights.clear();
}

void SceneMesh::AddLight(iLight* light)
{
  if (light)
  {
    light->AddRef();
    m_lights.push_back(light);
  }
}

}