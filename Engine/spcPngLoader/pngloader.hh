
#pragma once

#include <spcPngLoader/pngloaderexport.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc
{


SPC_CLASS()
class SPC_PNGLOADER_API PngLoader: public SPC_SUPER(iAssetLoader)
{
  SPC_CLASS_GEN_OBJECT;
public:
  PngLoader();

  virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;

};


}