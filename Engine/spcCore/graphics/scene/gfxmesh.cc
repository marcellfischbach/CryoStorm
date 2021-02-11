
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <spcCore/graphics/material/imaterial.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/idevice.hh>


namespace spc
{

GfxMesh::GfxMesh()
  : iObject()
  , m_material(nullptr)
  , m_mesh(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
}


GfxMesh::~GfxMesh()
{
  SPC_RELEASE(m_material);
  SPC_RELEASE(m_mesh);
  ClearLights();
}

void GfxMesh::Render(iDevice* device, eRenderPass pass)
{
  if (device->BindMaterial(m_material, pass))
  {
    if (pass == eRP_Forward)
    {
      Size i = 0; 
      for (GfxLight* light : m_lights)
      {
        device->BindForwardLight(light->GetLight(), i++);
      }
      device->FinishForwardLights(i);
    }

    device->SetModelMatrix(m_modelMatrix);
    device->Render(m_mesh, pass);
  }
}

void GfxMesh::SetStatic(bool _static)
{
  m_static = _static;

}

bool GfxMesh::IsStatic() const
{
  return m_static;
}

void GfxMesh::SetMaterial(iMaterial* material)
{
  SPC_SET(m_material, material);
}

iMaterial* GfxMesh::GetMaterial()
{
  return m_material;
}

const iMaterial* GfxMesh::GetMaterial() const
{
  return m_material;
}


void GfxMesh::SetMesh(iRenderMesh* mesh)
{
  SPC_SET(m_mesh, mesh);
}

iRenderMesh* GfxMesh::GetMesh()
{
  return m_mesh;
}

const iRenderMesh* GfxMesh::GetMesh() const
{
  return m_mesh;
}


void GfxMesh::SetModelMatrix(const Matrix4f& modelMatrix)
{
  m_modelMatrix = modelMatrix;
}

const Matrix4f& GfxMesh::GetModelMatrix()  const
{
  return m_modelMatrix;
}

void GfxMesh::ClearLights()
{
  for (GfxLight* light : m_lights)
  {
    light->Release();
  }
  m_lights.clear();
}

void GfxMesh::AddLight(GfxLight* light)
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

void GfxMesh::RemoveLight(GfxLight* light)
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

Size GfxMesh::GetNumberOfLights() const
{
  return m_lights.size();
}

const std::vector<GfxLight*>& GfxMesh::GetLights() const
{
  return m_lights;
}

}