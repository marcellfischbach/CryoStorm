

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/resource/csBaseCSFAssetLoader.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API csSamplerLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csSamplerLoader();
  virtual ~csSamplerLoader() { }


protected:
  iObject* Load(const csCryoFile *file, const Class* cls, const csResourceLocator& locator) const override ;

};


}