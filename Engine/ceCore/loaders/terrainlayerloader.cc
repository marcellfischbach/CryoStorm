//
// Created by MCEL on 07.07.2022.
//

#include <ceCore/loaders/terrainlayerloader.hh>
#include <ceCore/entity/csTerrainMeshState.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/iTexture2D.hh>


namespace cryo
{

bool TerrainLayerLoader::CanLoad(const Class* cls, const ResourceLocator& locator) const
{
  return cls ->IsAssignableFrom<csTerrainLayer>() && locator.GetExtension() == std::string("TERRAINLAYER");
}

iObject* TerrainLayerLoader::Load(const CrimsonFile* file,
                                  const Class* cls,
                                  const ResourceLocator& locator) const
{
  const CrimsonFileElement* rootElement = file->Root();

  const CrimsonFileElement* terrainLayerElement = rootElement->GetChild("terrainLayer");
  if (!terrainLayerElement)
  {
    return nullptr;
  }

  AssetManager* assetMan = AssetManager::Get();

  csTerrainLayer * layer = new csTerrainLayer();

  const CrimsonFileElement  * diffuseRoughnessElement   = terrainLayerElement->GetChild("diffuseRoughness");
  const CrimsonFileAttribute* diffuseRoughnessAttribute = diffuseRoughnessElement
                                                     ? diffuseRoughnessElement->GetAttribute(0)
                                                     : nullptr;
  if (diffuseRoughnessAttribute)
  {
    layer->SetDiffuseRoughness(assetMan->Get<iTexture2D>(ResourceLocator(locator, diffuseRoughnessAttribute->GetValue())));
  }

  const CrimsonFileElement  * normalElement   = terrainLayerElement->GetChild("normal");
  const CrimsonFileAttribute* normalAttribute = normalElement
                                           ? normalElement->GetAttribute(0)
                                           : nullptr;
  if (normalAttribute)
  {
    layer->SetNormal(assetMan->Get<iTexture2D>(ResourceLocator(locator, normalAttribute->GetValue())));
  }

  return layer;
}






bool TerrainLayerMaskLoader::CanLoad(const Class* cls, const ResourceLocator& locator) const
{
  return cls ->IsAssignableFrom<csTerrainLayerMask>() && locator.GetExtension() == std::string("TERRAINMASK");

}

iObject* TerrainLayerMaskLoader::Load(const CrimsonFile* file,
                                      const Class* cls,
                                      const ResourceLocator& locator) const
{
  const CrimsonFileElement* rootElement = file->Root();

  const CrimsonFileElement* terrainLayerMaskElement = rootElement->GetChild("terrainLayerMask");
  if (!terrainLayerMaskElement)
  {
    return nullptr;
  }

  AssetManager* assetMan = AssetManager::Get();

  auto layer = new csTerrainLayerMask();

  const CrimsonFileElement  * layersElement   = terrainLayerMaskElement->GetChild("layers");
  const CrimsonFileAttribute* layersAttribute = layersElement
                                           ? layersElement->GetAttribute(0)
                                           : nullptr;
  if (layersAttribute)
  {
    layer->SetLayerTexture(assetMan->Get<iTexture2D>(ResourceLocator(locator, layersAttribute->GetValue())));
  }

  const CrimsonFileElement  * maskElement   = terrainLayerMaskElement->GetChild("mask");
  const CrimsonFileAttribute* maskAttribute = maskElement
                                           ? maskElement->GetAttribute(0)
                                           : nullptr;
  if (maskAttribute)
  {
    layer->SetMaskTexture(assetMan->Get<iTexture2D>(ResourceLocator(locator, maskAttribute->GetValue())));
  }

  return layer;
}

}