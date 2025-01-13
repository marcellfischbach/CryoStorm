
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/csAssetLoader.hh>
#include <csCore/resource/iAsset.hh>
#include <csCore/csRef.hh>
#include <map>
#include <vector>
#include <string>

namespace cs
{


class CS_CORE_API csAssetManager
{
  friend class csEngine;
public:
  static void Set(csAssetManager *manager);
  static csAssetManager *Get();

  void RegisterLoader(csAssetLoader* loader);

  template<typename T>
  T *Get(const std::string &locator)
  {
    iAsset *res = Get(csAssetLocator(locator));
    return res ? res->Query<T>() : nullptr;
  }
  template<typename T>
  T *Get(const csAssetLocator &locator)
  {
    iAsset *res = Get(locator);
    return res ? res->Query<T>() : nullptr;
  }
  iAsset *Get(const std::string &locator)
  {
    return Get(csAssetLocator(locator));
  }
  iAsset *Get(const csAssetLocator &locator);

  template<typename T>
  T *Load(const std::string &locator)
  {
    iAsset *res = Load(csAssetLocator(locator));
    return res ? res->Query<T>() : nullptr;
  }
  template<typename T>
  T *Load(const csAssetLocator &locator)
  {
    iAsset *res = Load(locator);
    return res ? res->Query<T>() : nullptr;
  }
  iAsset *Load(const std::string &locator)
  {
    return Load(csAssetLocator(locator));
  }


  iAsset *Load(const csAssetLocator &locator);

protected:
  csAssetManager();


  std::vector<csAssetLoader* > m_assetLoaders;


private:
  static csAssetManager *s_assetManager;
};


}