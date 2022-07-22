

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace ce
{


CE_CLASS()
class CE_CORE_API SamplerLoader : public CE_SUPER(BaseCEFAssetLoader)
{
  CE_CLASS_GEN_OBJECT;
public:
  SamplerLoader ();
  virtual ~SamplerLoader() { }


protected:
  iObject* Load(const CrimsonFile *file, const Class* cls, const ResourceLocator& locator) const override ;

};


}