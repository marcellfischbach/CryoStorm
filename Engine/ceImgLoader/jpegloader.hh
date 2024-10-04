
#pragma once

#include <ceImgLoader/imgloaderexport.hh>
#include <ceCore/resource/iAssetLoader.hh>
#include <png.h>

namespace cryo::img
{


  CS_CLASS()
    class CS_IMGLOADER_API JpegLoader : public CS_SUPER(iAssetLoader)
  {
    CS_CLASS_GEN_OBJECT;
  public:
    JpegLoader();
    virtual ~JpegLoader() { }

    virtual bool CanLoad(const Class * cls, const csResourceLocator & locator) const;

    virtual iObject* Load(const Class * cls, const csResourceLocator & locator) const;

  };


}