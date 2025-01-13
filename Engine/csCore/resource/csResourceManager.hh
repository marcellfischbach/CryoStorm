////
//// Created by Marcell on 12.01.2025.
////
//
//#pragma once
//
//#include <csCore/csCoreExport.hh>
//#include <csCore/resource/csAssetLocator.hh>
//#include <csCore/resource/iAsset.hh>
//#include <csCore/csRef.hh>
//#include <map>
//
//
//namespace cs
//{
//
//class CS_CORE_API csResourceManager
//{
//public:
//  static csResourceManager &Instance();
//
//
//
//  void AddResource(const csAssetLocator &locator, csAssetRef<iAsset> &resource);
//  csAssetRef<iAsset> GetResource(const csAssetLocator &locator);
//  csAssetRef<iAsset> Load(const csAssetLocator &locator) const;
//
//  template<typename T>
//  csAssetRef<T> GetResource(const csAssetLocator &locator)
//  {
//    csAssetRef<iAsset> res       = GetResource(locator);
//    iAsset             *resource = res.raw();
//    T* t = csQueryClass<T>(resource);
//    return csAssetRef<T>(t);
//  }
//
//  template<typename T>
//  csAssetRef<T> GetLoad(const csAssetLocator &locator)
//  {
//    csAssetRef<iAsset> res       = Load(locator);
//    iAsset             *resource = res.raw();
//    T* t = csQueryClass<T>(resource);
//    return csAssetRef<T>(t);
//  }
//
//private:
//  csResourceManager() = default;
//  ~csResourceManager() = default;
//
//  std::map<csAssetLocator, csRef<iAsset>> m_resources;
//};
//
//} // cs
