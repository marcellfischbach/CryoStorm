

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/basecefassetloader.hh>

namespace ce
{


CS_CLASS()
class CS_CORE_API SamplerLoader : public CS_SUPER(BaseCEFAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  SamplerLoader ();
  virtual ~SamplerLoader() { }


protected:
  iObject* Load(const CrimsonFile *file, const Class* cls, const ResourceLocator& locator) const override ;

};


}