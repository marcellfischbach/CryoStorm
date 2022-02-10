
#pragma once

#include <ceImgLoader/imgloaderexport.hh>
#include <ceCore/resource/iassetloader.hh>

namespace ce::img
{


CE_CLASS()
class CE_IMGLOADER_API PngLoader: public CE_SUPER(iAssetLoader)
{
  CE_CLASS_GEN_OBJECT;
public:
  PngLoader();
  virtual ~PngLoader() { }

  virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;


};


}