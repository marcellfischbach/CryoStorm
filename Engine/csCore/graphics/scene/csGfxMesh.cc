
#include <csCore/graphics/scene/csGfxMesh.hh>
#include <csCore/graphics/material/iMaterial.hh>
#include <csCore/graphics/iRenderMesh.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/csSkeleton.hh>

#include <algorithm>


namespace cs
{

csGfxMesh::csGfxMesh()
  : iObject()
  , m_skeleton(nullptr)
{
  CS_CLASS_GEN_CONSTR;
}


csGfxMesh::~csGfxMesh()
{
  CS_RELEASE(m_material);
  CS_RELEASE(m_mesh);
  ClearLights();
}

void csGfxMesh::Render(iDevice* device, eRenderPass pass)
{
  //printf ("Prepare(mat: %p)\n", m_material);
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
      const std::vector<csMatrix4f> &bones = m_skeleton->GetSkeletonBones();
      device->SetSkeletonMatrices(bones.data(), bones.size());
    }
    device->SetModelMatrix(m_modelMatrix);
    device->Render(m_mesh, pass);
  }
}


void csGfxMesh::RenderUnlit(iDevice* device, eRenderPass pass)
{
  //printf ("RenderUnlit(mat: %p)\n", m_material);
  if (device->BindMaterial(m_material, pass))
  {
    if (m_skeleton)
    {
      const std::vector<csMatrix4f> &bones = m_skeleton->GetSkeletonBones();
      device->SetSkeletonMatrices(bones.data(), bones.size());
    }

    device->SetModelMatrix(m_modelMatrix);
    device->Render(m_mesh, pass);
  }
}

void csGfxMesh::RenderForward(iDevice* device, eRenderPass pass, const csGfxLight** lights, Size numberOfLights)
{

  //printf ("RenderForward(mat: %p)\n", m_material);
  if (device->BindMaterial(m_material, pass))
  {
    Size i;
    for (i = 0; i < numberOfLights; ++i)
    {
      const csGfxLight * gfxLight = lights[i];
      const iLight     * light    = gfxLight ? gfxLight->GetLight() : nullptr;
      device->BindForwardLight(light, i);
    }
    device->FinishForwardLights(i);

    if (m_skeleton)
    {
      const std::vector<csMatrix4f> &bones = m_skeleton->GetSkeletonBones();
      device->SetSkeletonMatrices(bones.data(), bones.size());
    }

    device->SetModelMatrix(m_modelMatrix);
    device->Render(m_mesh, pass);
    //device->SetShader(nullptr);
  }
}


void csGfxMesh::SetBatchable(bool batchable)
{
  m_batchable = batchable;
}

bool csGfxMesh::IsBatchable() const
{
  return m_batchable;
}

void csGfxMesh::SetStatic(bool _static)
{
  m_static = _static;
}

bool csGfxMesh::IsStatic() const
{
  return m_static;
}

void csGfxMesh::SetReceiveShadow(bool receiveShadow)
{
  m_receiveShadow = receiveShadow;
}

bool csGfxMesh::IsReceiveShadow() const
{
  return m_receiveShadow;
}


void csGfxMesh::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
}

bool csGfxMesh::IsCastShadow() const
{
  return m_castShadow;
}


void csGfxMesh::SetMaterial(iMaterial* material)
{
  CS_SET(m_material, material);
}

iMaterial* csGfxMesh::GetMaterial()
{
  return m_material;
}

const iMaterial* csGfxMesh::GetMaterial() const
{
  return m_material;
}


void csGfxMesh::SetMesh(iRenderMesh* mesh)
{
  CS_SET(m_mesh, mesh);
  UpdateBoundingBox();
}

iRenderMesh* csGfxMesh::GetMesh()
{
  return m_mesh;
}

const iRenderMesh* csGfxMesh::GetMesh() const
{
  return m_mesh;
}


void csGfxMesh::SetModelMatrix(const csMatrix4f& modelMatrix)
{
  m_modelMatrix = modelMatrix;
  UpdateBoundingBox();
}

const csMatrix4f& csGfxMesh::GetModelMatrix()  const
{
  return m_modelMatrix;
}

void csGfxMesh::ClearLights()
{
  for (Light& light : m_lights)
  {
    light.Light->Release();
  }
  m_lights.clear();
}

void csGfxMesh::AddLight(csGfxLight* light, float influence)
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

void csGfxMesh::RemoveLight(csGfxLight* light)
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

void csGfxMesh::SortAndLimitLights(Size size)
{
  std::sort(m_lights.begin(), m_lights.end(), [](Light& l0, Light& l1) { return l0.Influence > l1.Influence; });
  if (m_lights.size() > 4)
  {
    m_lights.resize(size);
  }
}

Size csGfxMesh::GetNumberOfLights() const
{
  return m_lights.size();
}

const std::vector<csGfxMesh::Light>& csGfxMesh::GetLights() const
{
  return m_lights;
}

const csBoundingBox &csGfxMesh::GetBoundingBox() const
{
  return m_boundingBox;
}

void csGfxMesh::SetFrame(uint64_t frame)
{
  m_frame = frame;
}

uint64_t csGfxMesh::GetFrame() const
{
  return m_frame;
}

void csGfxMesh::SetLightingDirty(bool lightingDirty)
{
  m_lightingDirty = lightingDirty;
}

bool csGfxMesh::IsLightingDirty() const
{
  return m_lightingDirty;
}

void csGfxMesh::SetSkeleton(cs::csSkeleton *skeleton)
{
  CS_SET(m_skeleton, skeleton);
}

csSkeleton* csGfxMesh::GetSkeleton()
{
  return m_skeleton;
}

const csSkeleton* csGfxMesh::GetSkeleton() const
{
  return m_skeleton;
}

void csGfxMesh::UpdateBoundingBox()
{
  m_boundingBox.Clear();
  if (m_mesh)
  {
    m_boundingBox.Add(m_modelMatrix, m_mesh->GetBoundingBox());
  }
  m_boundingBox.Finish();
}

}