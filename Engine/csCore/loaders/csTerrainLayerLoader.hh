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
  csTerrainLayerLoader() = default;
  ~csTerrainLayerLoader() override = default;

  bool CanLoad(const csClass* cls, const csResourceLocator& locator) const override;

protected:
  iObject* Load(const csCryoFile* file, const csClass* cls, const csResourceLocator& locator) const override;

};





CS_CLASS()
class CS_CORE_API csTerrainLayerMaskLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
CS_CLASS_GEN;
public:
  csTerrainLayerMaskLoader() = default;
  ~csTerrainLayerMaskLoader() override = default;

  bool CanLoad(const csClass* cls, const csResourceLocator& locator) const override;

protected:
  iObject* Load(const csCryoFile* file, const csClass* cls, const csResourceLocator& locator) const override;

};

}