

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/iAsset.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <map>
#include <string>

namespace cs
{


class CS_CORE_API csAssetPool
{
public:
  static csAssetPool &Instance();

  template<typename T>
  T *Get(const csAssetLocator &locator)
  {
    iAsset *resource = Get(locator);
    return resource ? resource->Query<T>() : nullptr;
  }

  iAsset *Get(const csAssetLocator &locator);

  void Erase(const csAssetLocator &locator);
  void Put(const csAssetLocator &locator, iAsset *resource);
  void Put(iAsset *resource);

  void Clear();

private:
  csAssetPool();

  //
  // Needs to be a native pointer not an csRef<iAsset> because the class csRef includes csAssetPool so we would
  // have a cyclic include
  std::map<std::string, iAsset*> m_resources;
};


}