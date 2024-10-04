//
// Created by Marcell on 30.12.2023.
//

#include <csCore/graphics/scene/csGfxMeshCompound.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/graphics/iRenderMesh.hh>

namespace cryo
{

csGfxMeshCompound::csGfxMeshCompound()
: csGfxMesh()
{
  
}

csGfxMeshCompound::~csGfxMeshCompound()
{
  
}

void csGfxMeshCompound::AddMesh(cryo::csGfxMesh *mesh)
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

void csGfxMeshCompound::RemoveMesh(cryo::csGfxMesh *mesh)
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

bool csGfxMeshCompound::ContainsMesh(const cryo::csGfxMesh *mesh) const
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

void csGfxMeshCompound::RegenerateMesh()
{
  printf ("Regenerate mesh: %d\n", m_internalMeshes.size());
  iMaterial* material = nullptr;
  iRenderMeshBatchGenerator *generator = csObjectRegistry::Get<iRenderMeshBatchGeneratorFactory>()->Create();
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