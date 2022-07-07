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

iObject* TerrainLayerLoader::Load(const file::File* file,
                                  const Class* cls,
                                  const ResourceLocator& locator) const
{
  const file::Element* rootElement = file->Root();

  const file::Element* terrainLayerElement = rootElement->GetChild("terrainLayer");
  if (!terrainLayerElement)
  {
    return nullptr;
  }

  AssetManager* assetMan = AssetManager::Get();

  TerrainLayer* layer = new TerrainLayer();

  const file::Element  * diffuseRoughnessElement   = terrainLayerElement->GetChild("diffuseRoughness");
  const file::Attribute* diffuseRoughnessAttribute = diffuseRoughnessElement
                                                     ? diffuseRoughnessElement->GetAttribute(0)
                                                     : nullptr;
  if (diffuseRoughnessAttribute)
  {
    layer->SetDiffuseRoughness(assetMan->Get<iTexture2D>(ResourceLocator(locator, diffuseRoughnessAttribute->GetValue())));
  }

  const file::Element  * normalElement   = terrainLayerElement->GetChild("normal");
  const file::Attribute* normalAttribute = normalElement
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

iObject* TerrainLayerMaskLoader::Load(const file::File* file,
                                      const Class* cls,
                                      const ResourceLocator& locator) const
{
  const file::Element* rootElement = file->Root();

  const file::Element* terrainLayerMaskElement = rootElement->GetChild("terrainLayerMask");
  if (!terrainLayerMaskElement)
  {
    return nullptr;
  }

  AssetManager* assetMan = AssetManager::Get();

  auto layer = new TerrainLayerMask();

  const file::Element  * layersElement   = terrainLayerMaskElement->GetChild("layers");
  const file::Attribute* layersAttribute = layersElement
                                           ? layersElement->GetAttribute(0)
                                           : nullptr;
  if (layersAttribute)
  {
    layer->SetLayerTexture(assetMan->Get<iTexture2D>(ResourceLocator(locator, layersAttribute->GetValue())));
  }

  const file::Element  * maskElement   = terrainLayerMaskElement->GetChild("mask");
  const file::Attribute* maskAttribute = maskElement
                                           ? maskElement->GetAttribute(0)
                                           : nullptr;
  if (maskAttribute)
  {
    layer->SetMaskTexture(assetMan->Get<iTexture2D>(ResourceLocator(locator, maskAttribute->GetValue())));
  }

  return layer;
}

}