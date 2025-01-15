//
// Created by MCEL on 07.07.2022.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/csAssetLoader.hh>
#include <csCore/resource/csCryoFile.hh>
#include <vector>

namespace cs
{


CS_CLASS()
class CS_CORE_API csBaseCSFAssetLoader : public CS_SUPER(csAssetLoader)
{
  CS_CLASS_GEN_OBJECT;

protected:
  csBaseCSFAssetLoader() = default;

  virtual csOwned<iAsset> Load(const csCryoFile *file, const csAssetLocator &locator) const = 0;

public:
  ~csBaseCSFAssetLoader() override = default;

  CS_NODISCARD csOwned<iAsset> Load(const csAssetLocator &locator) const override;

private:
  CS_NODISCARD static bool Open(const csAssetLocator &locator, csCryoFile &file);


};

} // ce
