

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/iResource.hh>
#include <csCore/resource/csResourceLocator.hh>
#include <map>
#include <string>

namespace cs
{


class CS_CORE_API csResourcePool
{
public:
  static csResourcePool &Instance();

  template<typename T>
  T *Get(const csResourceLocator &locator)
  {
    iResource *resource = Get(locator);
    return resource ? resource->Query<T>() : nullptr;
  }

  iResource *Get(const csResourceLocator &locator);

  void Erase(const csResourceLocator &locator);
  void Put(const csResourceLocator &locator, iResource *resource);

private:
  csResourcePool();

  std::map<csResourceLocator, iResource *> m_resources;
};


}