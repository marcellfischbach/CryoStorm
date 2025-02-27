//
// Created by Marcell on 27.02.2025.
//

#include <csCore/loaders/csMeshLoader.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/resource/csAssetManager.hh>

namespace cs
{


csMeshLoader::csMeshLoader()
    : csBaseCSFAssetLoader()
{
  RegisterType("MESH");
}

csOwned<iAsset> csMeshLoader::Load(const csCryoFile *file, const csAssetLocator &locator) const
{
  const csCryoFileElement *meshElement = file->Root()->GetChild("mesh");
  if (!meshElement)
  {
    return csOwned<iAsset>();
  }

  const csCryoFileElement *materialSlotsElement = meshElement->GetChild("materialSlots");
  const csCryoFileElement *meshesElement        = meshElement->GetChild("meshes");
  if (!materialSlotsElement || !meshesElement)
  {
    return csOwned<iAsset>();
  }

  csAssetManager   *man               = csAssetManager::Get();

  csMesh *mesh = new csMesh();
  for (int i = 0; i < materialSlotsElement->GetNumberOfChildren(); ++i)
  {
    const csCryoFileElement *materialSlotElement = materialSlotsElement->GetChild(i);
    if (materialSlotElement->GetTagName() != "materialSlot")
    {
      continue;
    }

    std::string      slotName  = materialSlotElement->GetAttribute("name", "Def");
    std::string      matLocStr = materialSlotElement->GetAttribute("locator", "/narf");
    csRef<iMaterial> material  = man->Get<iMaterial>(csAssetLocator(locator, matLocStr));

    mesh->AddMaterialSlot(slotName, material);
  }

  for (int i = 0; i < meshesElement->GetNumberOfChildren(); ++i)
  {
    const csCryoFileElement *meshElement = meshesElement->GetChild(i);
    if (meshElement->GetTagName() != "mesh")
    {
      continue;
    }

    int         slot              = meshElement->GetAttribute("slot", 0);
    std::string meshLocStr        = meshElement->GetAttribute("locator", "/narf");
    csRef<iRenderMesh> rmesh = man->Get<iRenderMesh>(csAssetLocator(locator, meshLocStr));

    mesh->AddSubMesh(rmesh, slot);
  }


  return csOwned<csMesh>(mesh);
}

} // cs