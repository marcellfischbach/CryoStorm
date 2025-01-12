//
// Created by Marcell on 12.01.2025.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/csResourceLocator.hh>
#include <csCore/resource/iResource.hh>
#include <csCore/csRef.hh>
#include <map>


namespace cs
{

class CS_CORE_API csResourceManager
{
public:
  static csResourceManager &Instance();



  void AddResource(const csResourceLocator &locator, csRes<iResource> &resource);
  csRes<iResource> GetResource(const csResourceLocator &locator);
  csRes<iResource> Load (const csResourceLocator &locator) const;

  template<typename T>
  csRes<T> GetResource (const csResourceLocator &locator)
  {
    csRes<iResource> res = GetResource(locator);
    iResource *resource = res.raw();
    T* t = csQueryClass<T>(resource);
    return csRes<T>(t);
  }

  template<typename T>
  csRes<T> GetLoad (const csResourceLocator &locator)
  {
    csRes<iResource> res = Load(locator);
    iResource *resource = res.raw();
    T* t = csQueryClass<T>(resource);
    return csRes<T>(t);
  }

private:
  csResourceManager() = default;
  ~csResourceManager() = default;

  std::map<csResourceLocator, csRef<iResource>> m_resources;
};

} // cs
