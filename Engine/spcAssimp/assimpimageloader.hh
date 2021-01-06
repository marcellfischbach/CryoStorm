
#pragma once

#include <spcAssimp/assimpexport.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc::assimp
{


SPC_CLASS()
class SPC_ASSIMP_API AssimpImageLoader : public SPC_SUPER(iAssetLoader)
{
  SPC_CLASS_GEN_OBJECT;
public:
  AssimpImageLoader();

  virtual bool CanLoad(const Class *cls, const ResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;

};


}