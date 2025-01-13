//
//#pragma once
//
//#include <csCore/csCoreExport.hh>
//#include <csCore/csClass.hh>
//#include <csCore/csTypes.hh>
//#include <csCore/resource/csAssetLocator.hh>
//
//namespace cs
//{
//
//const int16_t DefaultPriority = 0;
//
//CS_CLASS()
//struct CS_CORE_API iAssetLoader : public iObject
//{
//  CS_CLASS_GEN;
//  virtual ~iAssetLoader()
//  {
//
//  }
//
//  virtual int16_t Priority() const
//  {
//    return DefaultPriority;
//  }
//
//  virtual bool CanLoad(const csClass* cls, const csAssetLocator& locator) const = 0;
//
//  virtual iObject* Load(const csClass* cls, const csAssetLocator& locator) const = 0;
//
//};
//
//
//}