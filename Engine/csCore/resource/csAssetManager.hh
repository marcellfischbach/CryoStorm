
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
  csAssetRef<T> Get(const std::string &locator)
  {
    csAssetRef<iAsset> res = Get(csAssetLocator(locator));
    return res ? csAssetRef<T>(res->Query<T>()) : csAssetRef<T>::Null();
  }
  template<typename T>
  csAssetRef<T> Get(const csAssetLocator &locator)
  {
    csAssetRef<iAsset> res = Get(locator);
    return res ? csAssetRef<T>(res->Query<T>()) : csAssetRef<T>::Null();
  }
  csAssetRef<iAsset> Get(const std::string &locator)
  {
    return Get(csAssetLocator(locator));
  }
  csAssetRef<iAsset> Get(const csAssetLocator &locator);

  template<typename T>
  csAssetRef<T> Load(const std::string &locator)
  {
    csAssetRef<iAsset> res = Load(csAssetLocator(locator));
    return res ? csAssetRef<T>(res->Query<T>()) : csAssetRef<T>::Null();
  }
  template<typename T>
  csAssetRef<T> Load(const csAssetLocator &locator)
  {
    csAssetRef<iAsset> res = Load(locator);
    return res ? csAssetRef<T>(res->Query<T>()) : csAssetRef<T>::Null();
  }
  csAssetRef<iAsset> Load(const std::string &locator)
  {
    return Load(csAssetLocator(locator));
  }


  csAssetRef<iAsset> Load(const csAssetLocator &locator);

protected:
  csAssetManager();


  std::vector<csAssetLoader* > m_assetLoaders;


private:
  static csAssetManager *s_assetManager;
};


}