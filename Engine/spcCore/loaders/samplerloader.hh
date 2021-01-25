

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc
{


SPC_CLASS()
class SPC_CORE_API SamplerLoaderSpc : public SPC_SUPER(iAssetLoaderSpc)
{
  SPC_CLASS_GEN_OBJECT;
public:
  virtual ~SamplerLoaderSpc() { }


  bool CanLoad(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

  iObject* Load(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

};


}