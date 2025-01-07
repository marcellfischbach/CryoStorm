
#include <csCore/entity/csTerrainMeshState.hh>
#include <csCore/entity/csCameraState.hh>
#include <csCore/entity//csWorld.hh>
#include <csCore/graphics/iTerrainMesh.hh>
#include <csCore/graphics/iTexture2D.hh>
#include <csCore/graphics/material/iMaterial.hh>
#include <csCore/graphics/material/csMaterial.hh>
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/graphics/scene/csGfxMesh.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csResource_Impl.hh>



namespace cs
{



csTerrainLayerMask::~csTerrainLayerMask()
{
  CS_RELEASE(m_layerTexture);
  CS_RELEASE(m_maskTexture);
}

void csTerrainLayerMask::SetLayerTexture(iTexture2D* layerTexture)
{
  CS_SET(m_layerTexture, layerTexture);
}

iTexture2D *csTerrainLayerMask::GetLayerTexture()
{
  return m_layerTexture;
}

const iTexture2D *csTerrainLayerMask::GetLayerTexture() const
{
  return m_layerTexture;
}


void csTerrainLayerMask::SetMaskTexture(iTexture2D* maskTexture)
{
  CS_SET(m_maskTexture, maskTexture);
}

iTexture2D *csTerrainLayerMask::GetMaskTexture()
{
  return m_maskTexture;
}

const iTexture2D *csTerrainLayerMask::GetMaskTexture() const
{
  return m_maskTexture;
}





csTerrainLayer::~csTerrainLayer()
{
  CS_RELEASE(m_diffuseRoughness);
  CS_RELEASE(m_normal);
}

void csTerrainLayer::SetDiffuseRoughness(iTexture2D* diffuseRoughness)
{
  CS_SET(m_diffuseRoughness, diffuseRoughness);
}

iTexture2D *csTerrainLayer::GetDiffuseRoughness()
{
  return m_diffuseRoughness;
}

const iTexture2D *csTerrainLayer::GetDiffuseRoughness() const
{
  return m_diffuseRoughness;
}


void csTerrainLayer::SetNormal(iTexture2D* normal)
{
  CS_SET(m_normal, normal);
}

iTexture2D *csTerrainLayer::GetNormal()
{
  return m_normal;
}

const iTexture2D *csTerrainLayer::GetNormal() const
{
  return m_normal;
}




csTerrainMeshState::csTerrainMeshState()
  : csSpatialState()
{
  SetNeedUpdate(true);
}

csTerrainMeshState::~csTerrainMeshState()
{
  CS_RELEASE(m_terrainMesh);
  CS_RELEASE(m_gfxMesh);
//  CS_RELEASE(m_material);
}

void csTerrainMeshState::SetTerrainMesh(iTerrainMesh* terrainMesh)
{
  CS_SET(m_terrainMesh, terrainMesh);

  csWorld * world = GetWorld();
  if (m_terrainMesh && world)
  {
    RemoveMeshFromScene(world);
  }
  CS_SET(m_terrainMesh, terrainMesh);
  if (m_terrainMesh && world)
  {
    AddMeshToScene(world);
  }
}

iTerrainMesh* csTerrainMeshState::GetTerrainMesh()
{
  return m_terrainMesh;
}

const iTerrainMesh* csTerrainMeshState::GetTerrainMesh() const
{
  return m_terrainMesh;
}

void csTerrainMeshState::SetLayerMask(csTerrainLayerMask* mask)
{
  CS_SET(m_layerMask, mask);
  UpdateMaterial();
}

void csTerrainMeshState::AddLayer(csTerrainLayer* layer)
{
  auto it = std::ranges::find(m_layers.begin(), m_layers.end(), layer);
  if (it != m_layers.end())
  {
    return;
  }

  CS_ADDREF(layer);
  m_layers.emplace_back(layer);

  UpdateMaterial();
}

void csTerrainMeshState::SetCastShadow(bool castShadow)
{
  m_castShadow = castShadow;
  if (m_gfxMesh)
  {
    m_gfxMesh->SetCastShadow(m_castShadow);
  }
}

bool csTerrainMeshState::IsCastShadow() const
{
  return m_castShadow;
}

void csTerrainMeshState::SetReceiveShadow(bool receiveShadow)
{
  m_receiveShadow = receiveShadow;
  if (m_gfxMesh)
  {
    m_gfxMesh->SetReceiveShadow(m_receiveShadow);
  }
}

bool csTerrainMeshState::IsReceiveShadow() const
{
  return m_receiveShadow;
}

void csTerrainMeshState::SetReference(csSpatialState* reference)
{
  CS_SET(m_reference, reference);
}

csSpatialState *csTerrainMeshState::GetReference()
{
  return m_reference;
}

const csSpatialState *csTerrainMeshState::GetReference() const
{
  return m_reference;
}

void csTerrainMeshState::OnAttachedToWorld(csWorld* world)
{
  if (m_terrainMesh)
  {
    AddMeshToScene(world);
  }
}

void csTerrainMeshState::OnDetachedFromWorld(csWorld* world)
{
  if (m_terrainMesh)
  {
    RemoveMeshFromScene(world);
  }
}

void csTerrainMeshState::Update(float tpf)
{
  const csSpatialState *reference = nullptr;
  if (m_reference)
  {
    reference = m_reference;
  }
  else
  {
    csWorld * world = GetWorld();
    if (world)
    {
      reference = world->GetMainCamera();
    }
  }

  if (m_terrainMesh && reference)
  {
    m_terrainMesh->SetReferencePoint(reference->GetGlobalMatrix().GetTranslation());
  }
}

void csTerrainMeshState::AddMeshToScene(csWorld* world)
{
  if (m_terrainMesh && m_material && world)
  {
    iGfxScene* scene = world->GetScene();

    m_gfxMesh = new csGfxMesh();
    m_gfxMesh->SetStatic(IsStatic());
    m_gfxMesh->SetReceiveShadow(m_receiveShadow);
    m_gfxMesh->SetCastShadow(m_castShadow);;
    m_gfxMesh->SetMesh(m_terrainMesh);
    m_gfxMesh->SetMaterial(m_material);
    m_gfxMesh->SetModelMatrix(GetGlobalMatrix());

    scene->Add(m_gfxMesh);
  }
}

void csTerrainMeshState::RemoveMeshFromScene(csWorld* world)
{
  if (m_gfxMesh && world)
  {
    iGfxScene * scene = world->GetScene();
    scene->Remove(m_gfxMesh);
    m_gfxMesh->Release();
  }
}

void csTerrainMeshState::TransformationUpdatedPreChildren()
{
  if (m_gfxMesh)
  {
    csMatrix4f mat = GetGlobalMatrix();
    m_gfxMesh->SetModelMatrix(mat);
  }
}

void csTerrainMeshState::UpdateMaterial()
{
  if (!m_material)
  {
    m_material = csAssetManager::Get()->Get<iMaterial>(csResourceLocator("/materials/terrain/terrain4.mat"));
//    const csResource<iMaterial> resMat (rawMaterial);
//    const iMaterial* resMatRaw = resMat.rawPtr();
//    m_material = resmaterial;
    if (!m_material)
    {
      return;
    }
  }

  printf ("m_layerMask: %p\n", m_layerMask);
  fflush(stdout);
  m_material->SetTexture(m_material->IndexOf("Layer"), m_layerMask->GetLayerTexture());

  for (size_t i=0; i<4; i++)
  {
    if (m_layers.size() > i)
    {
      std::string strDiffuseRoughness = std::string("DiffuseRoughness") + std::to_string(i);
      std::string strNormal           = std::string("Normal") + std::to_string(i);
      csTerrainLayer *& layer = m_layers[i];
      m_material->SetTexture(m_material->IndexOf(strDiffuseRoughness), layer->GetDiffuseRoughness());
      m_material->SetTexture(m_material->IndexOf(strNormal), layer->GetNormal());

    }
  }

  if (m_gfxMesh)
  {
    m_gfxMesh->SetMaterial(m_material);
  }

}

} // ce