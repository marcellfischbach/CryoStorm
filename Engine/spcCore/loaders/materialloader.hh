#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/resource/iassetloader.hh>

namespace spc
{


SPC_CLASS()
class SPC_CORE_API MaterialLoaderSpc : public SPC_SUPER(iAssetLoaderSpc)
{
SPC_CLASS_GEN_OBJECT;
public:
  virtual ~MaterialLoaderSpc() { }


  bool CanLoad(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

  iObject* Load(const Class * cls, const file::File * file, const ResourceLocator * locator) const override;

private:
  iObject* LoadMaterial(const Class * cls, const file::File * file, const ResourceLocator * locator) const;
  iObject* LoadMaterialInstance(const Class * cls, const file::File * file, const ResourceLocator * locator) const;

};


}