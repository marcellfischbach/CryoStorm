
#pragma once

#include <spcAssimp/assimpexport.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc
{


SPC_CLASS()
class SPC_ASSIMP_API AssimpImageLoader : public SPC_SUPER(iAssetLoader)
{
  SPC_CLASS_GEN_OBJECT;
public:
  AssimpImageLoader();

  virtual bool CanLoad(const ResourceLocator & locator) const;

  virtual iObject* Load(const ResourceLocator & locator) const;

};


}