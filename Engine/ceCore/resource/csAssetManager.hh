
#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/csClass.hh>
#include <ceCore/resource/iAssetLoader.hh>
#include <ceCore/resource/iAssetLoader.hh>
#include <map>
#include <vector>
#include <string>

namespace cryo
{


class CS_CORE_API csAssetManager
{
  friend class csEngine;
public:
  static void Set(csAssetManager *manager);
  static csAssetManager *Get();

  void RegisterLoader(iAssetLoader *loader);


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

protected:
  csAssetManager();


  std::vector<iAssetLoader *> m_loaders;

  std::map<csResourceLocator, iObject*> m_cachedObjects;

private:
  static csAssetManager *s_assetManager;
};


}