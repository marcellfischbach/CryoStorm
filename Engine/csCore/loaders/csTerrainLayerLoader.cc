//
// Created by MCEL on 07.07.2022.
//

#include <csCore/loaders/csTerrainLayerLoader.hh>
#include <csCore/entity/csTerrainMeshState.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/graphics/iTexture2D.hh>


namespace cryo
{

bool csTerrainLayerLoader::CanLoad(const csClass* cls, const csResourceLocator& locator) const
{
  return cls ->IsAssignableFrom<csTerrainLayer>() && locator.GetExtension() == std::string("TERRAINLAYER");
}

iObject* csTerrainLayerLoader::Load(const csCryoFile* file,
                                    const csClass* cls,
                                    const csResourceLocator& locator) const
{
  const csCryoFileElement * rootElement = file->Root();

  const csCryoFileElement * terrainLayerElement = rootElement->GetChild("terrainLayer");
  if (!terrainLayerElement)
  {
    return nullptr;
  }

  csAssetManager * assetMan = csAssetManager::Get();

  csTerrainLayer * layer = new csTerrainLayer();

  const csCryoFileElement   * diffuseRoughnessElement   = terrainLayerElement->GetChild("diffuseRoughness");
  const csCryoFileAttribute * diffuseRoughnessAttribute = diffuseRoughnessElement
                                                     ? diffuseRoughnessElement->GetAttribute(0)
                                                     : nullptr;
  if (diffuseRoughnessAttribute)
  {
    layer->SetDiffuseRoughness(assetMan->Get<iTexture2D>(csResourceLocator(locator, diffuseRoughnessAttribute->GetValue())));
  }

  const csCryoFileElement   * normalElement   = terrainLayerElement->GetChild("normal");
  const csCryoFileAttribute * normalAttribute = normalElement
                                           ? normalElement->GetAttribute(0)
                                           : nullptr;
  if (normalAttribute)
  {
    layer->SetNormal(assetMan->Get<iTexture2D>(csResourceLocator(locator, normalAttribute->GetValue())));
  }

  return layer;
}






bool csTerrainLayerMaskLoader::CanLoad(const csClass* cls, const csResourceLocator& locator) const
{
  return cls ->IsAssignableFrom<csTerrainLayerMask>() && locator.GetExtension() == std::string("TERRAINMASK");

}

iObject* csTerrainLayerMaskLoader::Load(const csCryoFile* file,
                                        const csClass* cls,
                                        const csResourceLocator& locator) const
{
  const csCryoFileElement * rootElement = file->Root();

  const csCryoFileElement * terrainLayerMaskElement = rootElement->GetChild("terrainLayerMask");
  if (!terrainLayerMaskElement)
  {
    return nullptr;
  }

  csAssetManager * assetMan = csAssetManager::Get();

  auto layer = new csTerrainLayerMask();

  const csCryoFileElement   * layersElement   = terrainLayerMaskElement->GetChild("layers");
  const csCryoFileAttribute * layersAttribute = layersElement
                                           ? layersElement->GetAttribute(0)
                                           : nullptr;
  if (layersAttribute)
  {
    layer->SetLayerTexture(assetMan->Get<iTexture2D>(csResourceLocator(locator, layersAttribute->GetValue())));
  }

  const csCryoFileElement   * maskElement   = terrainLayerMaskElement->GetChild("mask");
  const csCryoFileAttribute * maskAttribute = maskElement
                                           ? maskElement->GetAttribute(0)
                                           : nullptr;
  if (maskAttribute)
  {
    layer->SetMaskTexture(assetMan->Get<iTexture2D>(csResourceLocator(locator, maskAttribute->GetValue())));
  }

  return layer;
}

}