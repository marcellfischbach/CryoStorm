//
// Created by MCEL on 07.07.2022.
//

#include <ceCore/loaders/terrainlayerloader.hh>
#include <ceCore/entity/terrainmeshstate.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/itexture2d.hh>


namespace ce
{

bool TerrainLayerLoader::CanLoad(const Class* cls, const ResourceLocator& locator) const
{
  return cls ->IsAssignableFrom<TerrainLayer>() && locator.GetExtension() == std::string("TERRAINLAYER");
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

  TerrainLayer* layer = new TerrainLayer();

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
  return cls ->IsAssignableFrom<TerrainLayerMask>() && locator.GetExtension() == std::string("TERRAINMASK");

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

  auto layer = new TerrainLayerMask();

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