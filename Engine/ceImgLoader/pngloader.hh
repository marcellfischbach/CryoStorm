
#pragma once

#include <ceImgLoader/imgloaderexport.hh>
#include <ceCore/resource/iassetloader.hh>

namespace ce::img
{


CS_CLASS()
class CS_IMGLOADER_API PngLoader: public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  PngLoader();
  virtual ~PngLoader() { }

  virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;


};


}