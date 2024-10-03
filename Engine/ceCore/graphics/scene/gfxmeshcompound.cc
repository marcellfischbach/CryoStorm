//
// Created by Marcell on 30.12.2023.
//

#include <ceCore/graphics/scene/gfxmeshcompound.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/graphics/irendermesh.hh>

namespace cryo
{

GfxMeshCompound::GfxMeshCompound()
: GfxMesh()
{
  
}

GfxMeshCompound::~GfxMeshCompound()
{
  
}

void GfxMeshCompound::AddMesh(cryo::GfxMesh *mesh)
{
  for (auto &internalMesh: m_internalMeshes)
  {
    if (internalMesh == mesh)
    {
      return;
    }
  }
  
  m_internalMeshes.emplace_back(mesh);
  mesh->AddRef();
}

void GfxMeshCompound::RemoveMesh(cryo::GfxMesh *mesh)
{
  for (auto it = m_internalMeshes.begin(); it != m_internalMeshes.end(); ++it)
  {
    if (*it == mesh)
    {
      m_internalMeshes.erase(it);
      mesh->Release();
    }
  }
}

bool GfxMeshCompound::ContainsMesh(const cryo::GfxMesh *mesh) const
{
  if (this == mesh)
  {
    return true;
  }
  for (auto &internalMesh : m_internalMeshes)
  {
    if (internalMesh == mesh)
    {
      return true;
    }
  }
  return false;
}

void GfxMeshCompound::RegenerateMesh()
{
  printf ("Regenerate mesh: %d\n", m_internalMeshes.size());
  iMaterial* material = nullptr;
  iRenderMeshBatchGenerator *generator = ObjectRegistry::Get<iRenderMeshBatchGeneratorFactory>()->Create();
  for (const auto &internalMesh: m_internalMeshes)
  {
    generator->Add(internalMesh->GetMesh(), internalMesh->GetModelMatrix());
    if (!material)
    {
      material = internalMesh->GetMaterial();
    }
  }
  iRenderMesh* renderMesh = generator->Generate();
  SetMesh(renderMesh);
  SetMaterial(material);
}

} // ce