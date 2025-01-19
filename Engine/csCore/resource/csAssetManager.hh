
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
  csOwned<T> Get(const std::string &locator)
  {
    csOwned<iAsset> res = Get(csAssetLocator(locator));
    iAsset* asset = res.Data();
    return asset ? csOwned<T>(asset->Query<T>()) : csOwned<T>(nullptr);
  }
  template<typename T>
  csOwned<T> Get(const csAssetLocator &locator)
  {
    csOwned<iAsset> res = Get(locator);
    iAsset* asset = res.Data();
    return asset ? csOwned<T>(asset->Query<T>()) : csOwned<T>(nullptr);
  }
  csOwned<iAsset> Get(const std::string &locator)
  {
    return Get(csAssetLocator(locator));
  }
  csOwned<iAsset> Get(const csAssetLocator &locator);

  template<typename T>
  csOwned<T> Load(const std::string &locator)
  {
    csOwned<iAsset> res = Load(csAssetLocator(locator));
    iAsset* asset = res.Data();
    return asset ? csOwned<T>(asset->Query<T>()) : csOwned<T>(nullptr);
  }
  template<typename T>
  csOwned<T> Load(const csAssetLocator &locator)
  {
    csOwned<iAsset> res = Load(locator);
    iAsset* asset = res.Data();
    return asset ? csOwned<T>(asset->Query<T>()) : csOwned<T>(nullptr);
  }
  csOwned<iAsset> Load(const std::string &locator)
  {
    return Load(csAssetLocator(locator));
  }


  csOwned<iAsset> Load(const csAssetLocator &locator);

protected:
  csAssetManager();


  std::vector<csRef<csAssetLoader>> m_assetLoaders;


private:
  static csAssetManager *s_assetManager;



};


}