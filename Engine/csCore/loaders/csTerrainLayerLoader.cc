//
// Created by MCEL on 07.07.2022.
//

#include <csCore/loaders/csTerrainLayerLoader.hh>
#include <csCore/entity/csTerrainMeshState.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/graphics/iTexture2D.hh>


namespace cs
{
csTerrainLayerLoader::csTerrainLayerLoader()
{
  RegisterType("TERRAINLAYER");
}

csOwned<iAsset>csTerrainLayerLoader::Load(const csCryoFile *file, const csAssetLocator &locator) const
{
  const csCryoFileElement *rootElement = file->Root();

  const csCryoFileElement *terrainLayerElement = rootElement->GetChild("terrainLayer");
  if (!terrainLayerElement)
  {
    return nullptr;
  }

  csAssetManager *assetMan = csAssetManager::Get();

  csTerrainLayer *layer = new csTerrainLayer();

  const csCryoFileElement   *diffuseRoughnessElement   = terrainLayerElement->GetChild("diffuseRoughness");
  const csCryoFileAttribute *diffuseRoughnessAttribute = diffuseRoughnessElement
                                                         ? diffuseRoughnessElement->GetAttribute(0)
                                                         : nullptr;
  if (diffuseRoughnessAttribute)
  {
    layer->SetDiffuseRoughness(assetMan->Get<iTexture2D>(csAssetLocator(locator, diffuseRoughnessAttribute->GetValue())).Data());
  }

  const csCryoFileElement   *normalElement   = terrainLayerElement->GetChild("normal");
  const csCryoFileAttribute *normalAttribute = normalElement
                                               ? normalElement->GetAttribute(0)
                                               : nullptr;
  if (normalAttribute)
  {
    layer->SetNormal(assetMan->Get<iTexture2D>(csAssetLocator(locator, normalAttribute->GetValue())).Data());
  }

  return layer;
}


csTerrainLayerMaskLoader::csTerrainLayerMaskLoader()
{
  RegisterType("TERRAINMASK");
}


csOwned<iAsset>csTerrainLayerMaskLoader::Load(const csCryoFile *file, const csAssetLocator &locator) const
{
  const csCryoFileElement *rootElement = file->Root();

  const csCryoFileElement *terrainLayerMaskElement = rootElement->GetChild("terrainLayerMask");
  if (!terrainLayerMaskElement)
  {
    return nullptr;
  }

  csAssetManager *assetMan = csAssetManager::Get();

  auto layer = new csTerrainLayerMask();

  const csCryoFileElement   *layersElement   = terrainLayerMaskElement->GetChild("layers");
  const csCryoFileAttribute *layersAttribute = layersElement
                                               ? layersElement->GetAttribute(0)
                                               : nullptr;
  if (layersAttribute)
  {
    layer->SetLayerTexture(assetMan->Get<iTexture2D>(csAssetLocator(locator, layersAttribute->GetValue())).Data());
  }

  const csCryoFileElement   *maskElement   = terrainLayerMaskElement->GetChild("mask");
  const csCryoFileAttribute *maskAttribute = maskElement
                                             ? maskElement->GetAttribute(0)
                                             : nullptr;
  if (maskAttribute)
  {
    layer->SetMaskTexture(assetMan->Get<iTexture2D>(csAssetLocator(locator, maskAttribute->GetValue())).Data());
  }

  return layer;
}

}