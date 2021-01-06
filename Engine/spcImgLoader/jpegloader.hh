
#pragma once

#include <spcImgLoader/imgloaderexport.hh>
#include <spcCore/resource/iassetloader.hh>
#include <png.h>

namespace spc::img
{


  SPC_CLASS()
    class SPC_IMGLOADER_API JpegLoader : public SPC_SUPER(iAssetLoader)
  {
    SPC_CLASS_GEN_OBJECT;
  public:
    JpegLoader();

    virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

    virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;

  };


}