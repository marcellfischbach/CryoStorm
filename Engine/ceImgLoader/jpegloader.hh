
#pragma once

#include <ceImgLoader/imgloaderexport.hh>
#include <ceCore/resource/iassetloader.hh>
#include <png.h>

namespace ce::img
{


  CE_CLASS()
    class CE_IMGLOADER_API JpegLoader : public CE_SUPER(iAssetLoader)
  {
    CE_CLASS_GEN_OBJECT;
  public:
    JpegLoader();
    virtual ~JpegLoader() { }

    virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

    virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;

  };


}