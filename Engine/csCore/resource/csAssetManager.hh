
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/csAssetLoader.hh>
#include <csCore/resource/iAssetLoader.hh>
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

  void RegisterLoader(iAssetLoader *loader);
  void RegisterLoader(csAssetLoader* loader);

  template<typename T>
  T *Get(const std::string &locator)
  {
    return Get<T>(csResourceLocator(locator));
  }
  iObject *Get(const csClass *cls, const std::string &locator)
  {
    return Get(cls, csResourceLocator(locator));
  }

  template<typename T>
  T *Get(const csResourceLocator &locator)
  {
    iObject *obj = Get(T::GetStaticClass(), locator);
    return obj ? obj->Query<T>() : nullptr;
  }
  iObject *Get(const csClass *cls, const csResourceLocator &locator);

  template<typename T>
  T *Load(const csResourceLocator &locator)
  {
    iObject *obj = Load(T::GetStaticClass(), locator);
    return obj ? obj->Query<T>() : nullptr;
  }
  iObject *Load(const csClass *cls, const csResourceLocator &locator);

  template<typename T>
  T *Load(const std::string &locator)
  {
    iObject *obj = Load(T::GetStaticClass(), csResourceLocator(locator));
    return obj ? obj->Query<T>() : nullptr;
  }
  iObject *Load(const csClass *cls, const std::string &locator)
  {
    return Load(cls, csResourceLocator(locator));
  }


  csRef<iObject> Load(const csResourceLocator& locator);

  template<typename T>
  csRef<T> LoadType(const csResourceLocator& locator)
  {
    csRef<iObject> obj = Load(locator);
    if (!obj)
    {
      return csRef<T>::Null();
    }
    return csRef<T>(obj.raw()->Query<T>());
  }

protected:
  csAssetManager();


  std::vector<iAssetLoader *> m_loaders;

  std::vector<csAssetLoader* > m_assetLoaders;
  std::vector<std::string> m_knownExtensions;

  std::map<csResourceLocator, iObject*> m_cachedObjects;

private:
  static csAssetManager *s_assetManager;
};


}