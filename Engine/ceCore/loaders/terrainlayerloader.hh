//
// Created by MCEL on 07.07.2022.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace  ce
{


CE_CLASS()
class CE_CORE_API TerrainLayerLoader : public CE_SUPER(BaseCEFAssetLoader)
{
  CE_CLASS_GEN;
public:
  TerrainLayerLoader() = default;
  ~TerrainLayerLoader() override = default;

  bool CanLoad(const Class* cls, const ResourceLocator& locator) const override;

protected:
  iObject* Load(const file::File* file, const Class* cls, const ResourceLocator& locator) const override;

};





CE_CLASS()
class CE_CORE_API TerrainLayerMaskLoader : public CE_SUPER(BaseCEFAssetLoader)
{
CE_CLASS_GEN;
public:
  TerrainLayerMaskLoader() = default;
  ~TerrainLayerMaskLoader() override = default;

  bool CanLoad(const Class* cls, const ResourceLocator& locator) const override;

protected:
  iObject* Load(const file::File* file, const Class* cls, const ResourceLocator& locator) const override;

};

}