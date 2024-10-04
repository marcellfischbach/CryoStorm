
#pragma once

#include <ceImgLoader/csImgLoaderExport.hh>
#include <ceCore/resource/iAssetLoader.hh>

namespace cryo::img
{


CS_CLASS()
class CS_IMGLOADER_API csPngLoader: public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csPngLoader();
  virtual ~csPngLoader() { }

  virtual bool CanLoad(const csClass * cls, const csResourceLocator & locator) const;

  virtual iObject* Load(const csClass * cls, const csResourceLocator & locator) const;


};


}