

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>

namespace cs
{


CS_CLASS()
class CS_CORE_API csSamplerLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csSamplerLoader();
  virtual ~csSamplerLoader() { }


protected:
  csOwned<iAsset> Load(const file::csCryoFile *file, const csAssetLocator &locator) const override ;

};


}