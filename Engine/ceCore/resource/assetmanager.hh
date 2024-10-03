
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/resource/iassetloader.hh>
#include <ceCore/resource/iassetloader.hh>
#include <map>
#include <vector>
#include <string>

namespace ce
{


class CS_CORE_API AssetManager
{
  friend class Engine;
public:
  static void Set(AssetManager *manager);
  static AssetManager *Get();

  void RegisterLoader(iAssetLoader *loader);


  template<typename T>
  T *Get(const std::string &locator)
  {
    return Get<T>(ResourceLocator(locator));
  }
  iObject *Get(const Class *cls, const std::string &locator)
  {
    return Get(cls, ResourceLocator(locator));
  }

  template<typename T>
  T *Get(const ResourceLocator &locator)
  {
    iObject *obj = Get(T::GetStaticClass(), locator);
    return obj ? obj->Query<T>() : nullptr;
  }
  iObject *Get(const Class *cls, const ResourceLocator &locator);

  template<typename T>
  T *Load(const ResourceLocator &locator)
  {
    iObject *obj = Load(T::GetStaticClass(), locator);
    return obj ? obj->Query<T>() : nullptr;
  }
  iObject *Load(const Class *cls, const ResourceLocator &locator);

  template<typename T>
  T *Load(const std::string &locator)
  {
    iObject *obj = Load(T::GetStaticClass(), ResourceLocator(locator));
    return obj ? obj->Query<T>() : nullptr;
  }
  iObject *Load(const Class *cls, const std::string &locator)
  {
    return Load(cls, ResourceLocator(locator));
  }

protected:
  AssetManager();


  std::vector<iAssetLoader *> m_loaders;

  std::map<ResourceLocator, iObject*> m_cachedObjects;

private:
  static AssetManager *s_assetManager;
};


}