
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/graphics/material/imaterial.hh>
#include <ceCore/graphics/irendermesh.hh>
#include <ceCore/graphics/idevice.hh>
#include <ceCore/graphics/skeleton.hh>

#include <algorithm>


namespace ce
{

GfxMesh::GfxMesh()
  : iObject()
  , m_skeleton(nullptr)
{
  CE_CLASS_GEN_CONSTR;
}


GfxMesh::~GfxMesh()
{
  CE_RELEASE(m_material);
  CE_RELEASE(m_mesh);
  ClearLights();
}

void GfxMesh::Render(iDevice* device, eRenderPass pass)
{
  //printf ("Render(mat: %p)\n", m_material);
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

    if (m_skeleton)
    {
      const std::vector<Matrix4f> &bones = m_skeleton->GetSkeletonBones();
      device->SetSkeletonMatrices(bones.data(), bones.size());
    }
    device->SetModelMatrix(m_modelMatrix);
    device->Render(m_mesh, pass);
  }
}


void GfxMesh::RenderUnlit(iDevice* device, eRenderPass pass)
{
  //printf ("RenderUnlit(mat: %p)\n", m_material);
  if (device->BindMaterial(m_material, pass))
  {
    if (m_skeleton)
    {
      const std::vector<Matrix4f> &bones = m_skeleton->GetSkeletonBones();
      device->SetSkeletonMatrices(bones.data(), bones.size());
    }

    device->SetModelMatrix(m_modelMatrix);
    device->Render(m_mesh, pass);
  }
}

void GfxMesh::RenderForward(iDevice* device, eRenderPass pass, const GfxLight** lights, Size numberOfLights)
{

  //printf ("RenderForward(mat: %p)\n", m_material);
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

    if (m_skeleton)
    {
      const std::vector<Matrix4f> &bones = m_skeleton->GetSkeletonBones();
      device->SetSkeletonMatrices(bones.data(), bones.size());
    }

    device->SetModelMatrix(m_modelMatrix);
    device->Render(m_mesh, pass);
    //device->SetShader(nullptr);
  }
}


void GfxMesh::SetBatchable(bool batchable)
{
  m_batchable = batchable;
}

bool GfxMesh::IsBatchable() const
{
  return m_batchable;
}

void GfxMesh::SetStatic(bool _static)
{
  m_static = _static;
}

bool GfxMesh::IsStatic() const
{
  return m_static;
}

void GfxMesh::SetReceiveShadow(bool receiveShadow)
{
  m_receiveShadow = receiveShadow;
}

bool GfxMesh::IsReceiveShadow() const
{
  return m_receiveShadow;
}


void GfxMesh::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

bool GfxMesh::IsCastShadow() const
{
  return m_castShadow;
}


void GfxMesh::SetMaterial(iMaterial* material)
{
  CE_SET(m_material, material);
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
  CE_SET(m_mesh, mesh);
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

void GfxMesh::SetFrame(uint64_t frame)
{
  m_frame = frame;
}

uint64_t GfxMesh::GetFrame() const
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

void GfxMesh::SetSkeleton(ce::Skeleton *skeleton)
{
  CE_SET(m_skeleton, skeleton);
}

Skeleton* GfxMesh::GetSkeleton()
{
  return m_skeleton;
}

const Skeleton* GfxMesh::GetSkeleton() const
{
  return m_skeleton;
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