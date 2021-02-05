
#include <spcCore/graphics/scene/gfxscenemesh.hh>
#include <spcCore/graphics/material/imaterial.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/idevice.hh>


namespace spc
{

GfxSceneMesh::GfxSceneMesh()
  : iObject()
  , m_material(nullptr)
  , m_mesh(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
}


GfxSceneMesh::~GfxSceneMesh()
{
  SPC_RELEASE(m_material);
  SPC_RELEASE(m_mesh);
  ClearLights();
}

void GfxSceneMesh::Render(iDevice* device, eRenderPass pass)
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

void GfxSceneMesh::SetMaterial(iMaterial* material)
{
  SPC_SET(m_material, material);
}

iMaterial* GfxSceneMesh::GetMaterial()
{
  return m_material;
}

const iMaterial* GfxSceneMesh::GetMaterial() const
{
  return m_material;
}


void GfxSceneMesh::SetMesh(iRenderMesh* mesh)
{
  SPC_SET(m_mesh, mesh);
}

iRenderMesh* GfxSceneMesh::GetMesh()
{
  return m_mesh;
}

const iRenderMesh* GfxSceneMesh::GetMesh() const
{
  return m_mesh;
}


void GfxSceneMesh::SetModelMatrix(const Matrix4f& modelMatrix)
{
  m_modelMatrix = modelMatrix;
}

const Matrix4f& GfxSceneMesh::GetModelMatrix()  const
{
  return m_modelMatrix;
}

void GfxSceneMesh::ClearLights()
{
  for (iLight* light : m_lights)
  {
    light->Release();
  }
  m_lights.clear();
}

void GfxSceneMesh::AddLight(iLight* light)
{
  if (light)
  {
    if (std::find(m_lights.begin(), m_lights.end(), light) != m_lights.end())
    {
      return;
    }
    light->AddRef();
    m_lights.push_back(light);
  }
}

void GfxSceneMesh::RemoveLight(iLight* light)
{
  if (light)
  {
    auto it = std::find(m_lights.begin(), m_lights.end(), light);
    if (it == m_lights.end())
    {
      return;
    }

    m_lights.erase(it);
    light->Release();
  }
}

}