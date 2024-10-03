//
// Created by MCEL on 07.07.2022.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API TerrainLayerLoader : public CS_SUPER(BaseCEFAssetLoader)
{
  CS_CLASS_GEN;
public:
  TerrainLayerLoader() = default;
  ~TerrainLayerLoader() override = default;

  bool CanLoad(const Class* cls, const ResourceLocator& locator) const override;

protected:
  iObject* Load(const CrimsonFile* file, const Class* cls, const ResourceLocator& locator) const override;

};





CS_CLASS()
class CS_CORE_API TerrainLayerMaskLoader : public CS_SUPER(BaseCEFAssetLoader)
{
CS_CLASS_GEN;
public:
  TerrainLayerMaskLoader() = default;
  ~TerrainLayerMaskLoader() override = default;

  bool CanLoad(const Class* cls, const ResourceLocator& locator) const override;

protected:
  iObject* Load(const CrimsonFile* file, const Class* cls, const ResourceLocator& locator) const override;

};

}