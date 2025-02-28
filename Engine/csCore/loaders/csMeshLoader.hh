
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csBaseCSFAssetLoader.hh>

namespace cs
{

CS_CLASS()
class csMeshLoader : public CS_SUPER(csBaseCSFAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  csMeshLoader();
  ~csMeshLoader() override = default;

protected:
  csOwned<iAsset> Load(const file::csCryoFile *file, const csAssetLocator &locator) const override ;

};

} // cs
