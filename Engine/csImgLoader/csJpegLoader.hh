
#pragma once

#include <csImgLoader/csImgLoaderExport.hh>
#include <csCore/resource/iAssetLoader.hh>
#include <png.h>

namespace cs::img
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