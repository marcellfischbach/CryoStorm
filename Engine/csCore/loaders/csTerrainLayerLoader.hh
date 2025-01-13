//
// Created by MCEL on 07.07.2022.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>

namespace cs
{


CS_CLASS()
class CS_CORE_API csTerrainLayerLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
  CS_CLASS_GEN;
public:
  csTerrainLayerLoader();
  ~csTerrainLayerLoader() override = default;


protected:
  iAsset *Load(const csCryoFile *file, const csAssetLocator &locator) const override;

};





CS_CLASS()
class CS_CORE_API csTerrainLayerMaskLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
CS_CLASS_GEN;
public:
  csTerrainLayerMaskLoader();
  ~csTerrainLayerMaskLoader() override = default;


protected:
  iAsset *Load(const csCryoFile *file, const csAssetLocator &locator) const override;

};

}