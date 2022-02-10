

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/iassetloader.hh>

namespace ce
{


CE_CLASS()
class CE_CORE_API SamplerLoaderCEF : public CE_SUPER(iAssetLoaderCEF)
{
  CE_CLASS_GEN_OBJECT;
public:
  virtual ~SamplerLoaderCEF() { }


  bool CanLoad(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

  iObject* Load(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

};


}