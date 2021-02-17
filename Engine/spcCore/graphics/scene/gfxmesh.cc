
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <spcCore/graphics/material/imaterial.hh>
#include <spcCore/graphics/irendermesh.hh>
#include <spcCore/graphics/idevice.hh>
#include <algorithm>


namespace spc
{

GfxMesh::GfxMesh()
  : iObject()
  , m_material(nullptr)
  , m_mesh(nullptr)
  , m_frame(0)
  , m_lightingDirty(true)
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
      for (Light &light : m_lights)
      {
        device->BindForwardLight(light.Light->GetLight(), i++);
      }
      device->FinishForwardLights(i);
    }

    device->SetModelMatrix(m_modelMatrix);
    device->Render(m_mesh, pass);
  }
}

void GfxMesh::RenderForward(iDevice* device, eRenderPass pass, const GfxLight** lights, Size numberOfLights)
{
  if (device->BindMaterial(m_material, pass))
  {
    Size i;
    for (i = 0; i < numberOfLights; ++i)
    {
      const GfxLight* gfxLight = lights[i];
      const iLight* light = gfxLight ? gfxLight->GetLight() : nullptr;
      device->BindForwardLight(light, i);
    }
    device->FinishForwardLights(i);

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
  UpdateBoundingBox();
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
  UpdateBoundingBox();
}

const Matrix4f& GfxMesh::GetModelMatrix()  const
{
  return m_modelMatrix;
}

void GfxMesh::ClearLights()
{
  for (Light& light : m_lights)
  {
    light.Light->Release();
  }
  m_lights.clear();
}

void GfxMesh::AddLight(GfxLight* light, float influence)
{
  if (light)
  {
    if (std::find(m_lights.begin(), m_lights.end(), light) != m_lights.end())
    {
      return;
    }
    Light lght = {};
    lght.Light = light;
    lght.Influence = influence;

    light->AddRef();
    m_lights.push_back(lght);
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
    m_lightingDirty = true;
  }
}

void GfxMesh::SortAndLimitLights(Size size)
{
  std::sort(m_lights.begin(), m_lights.end(), [](Light& l0, Light& l1) { return l0.Influence > l1.Influence; });
  if (m_lights.size() > 4)
  {
    m_lights.resize(size);
  }
}

Size GfxMesh::GetNumberOfLights() const
{
  return m_lights.size();
}

const std::vector<GfxMesh::Light>& GfxMesh::GetLights() const
{
  return m_lights;
}

const BoundingBox &GfxMesh::GetBoundingBox() const
{
  return m_boundingBox;
}

void GfxMesh::SetFrame(UInt64 frame)
{
  m_frame = frame;
}

UInt64 GfxMesh::GetFrame() const
{
  return m_frame;
}

void GfxMesh::SetLightingDirty(bool lightingDirty)
{
  m_lightingDirty = lightingDirty;
}

bool GfxMesh::IsLightingDirty() const
{
  return m_lightingDirty;
}


void GfxMesh::UpdateBoundingBox()
{
  m_boundingBox.Clear();
  if (m_mesh)
  {
    m_boundingBox.Add(m_modelMatrix, m_mesh->GetBoundingBox());
  }
  m_boundingBox.Finish();
}

}