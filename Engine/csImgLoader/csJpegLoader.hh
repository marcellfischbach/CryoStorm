
#pragma once

#include <csImgLoader/csImgLoaderExport.hh>
#include <csCore/resource/csAssetLoader.hh>
#include <png.h>

namespace cs::img
{


  CS_CLASS()
    class CS_IMGLOADER_API csJpegLoader : public CS_SUPER(csAssetLoader)
  {
    CS_CLASS_GEN_OBJECT;
  public:
    csJpegLoader();
    ~csJpegLoader() override = default;

    CS_NODISCARD csAssetRef<iAsset> Load(const csAssetLocator & locator) const override;

  };


}