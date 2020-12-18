#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc
{

SPC_CLASS()
class SPC_CORE_API MeshLoader : public SPC_SUPER(iAssetLoader)
{
  SPC_CLASS_GEN_OBJECT;
public:
  MeshLoader();
  virtual ~MeshLoader();


  virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;


};

}