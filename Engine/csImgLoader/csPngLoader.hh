
#pragma once

#include <csImgLoader/csImgLoaderExport.hh>
#include <csCore/resource/csAssetLoader.hh>

namespace cs::img
{


CS_CLASS()
class CS_IMGLOADER_API csPngLoader: public CS_SUPER(csAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csPngLoader();
  ~csPngLoader() override = default;

  CS_NODISCARD csAssetRef<iAsset> Load(const csAssetLocator & locator) const override;


};


}