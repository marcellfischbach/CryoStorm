

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


template<typename T>
class csResource
{
public:
  csResource(T *res = nullptr);
  csResource(const csResourceLocator &resourceLocator)
      : m_res(nullptr)
      , m_locator(resourceLocator)
  {
  }


  ~csResource();


  T *operator->()
  {
    if (!m_res || !m_res->IsValid())
    {
      T *resource = csResourcePool::Instance().Get<T>(m_locator);
      CS_SET(m_res, resource);
    }
    return m_res;
  }

  const T *operator->() const
  {
    if (!m_res || !m_res->IsValid())
    {
      T *resource = csResourcePool::Instance().Get<T>(m_locator);
      CS_SET(m_res, resource);
    }
    return m_res;
  }

  csResource<T> &operator=(T *res)
  {
    CS_SET(m_res, res);
    return *this;
  }


  operator T *()
  {
    return m_res;
  }

  operator const T *() const
  {
    return m_res;
  }

private:
  csResourceLocator m_locator;
  T *m_res;
};


}