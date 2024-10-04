
#pragma once

#include <ceImgLoader/csImgLoaderExport.hh>
#include <ceCore/resource/iAssetLoader.hh>
#include <png.h>

namespace cryo::img
{


  CS_CLASS()
    class CS_IMGLOADER_API csJpegLoader : public CS_SUPER(iAssetLoader)
  {
    CS_CLASS_GEN_OBJECT;
  public:
    csJpegLoader();
    virtual ~csJpegLoader() { }

    virtual bool CanLoad(const csClass * cls, const csResourceLocator & locator) const;

    virtual iObject* Load(const csClass * cls, const csResourceLocator & locator) const;

  };


}