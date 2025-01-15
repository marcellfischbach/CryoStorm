
#pragma once

#include <csCore/csClass.hh>
#include <csCore/csRef.hh>

namespace cs
{

template<typename T>
class csOwned
{
public:
  csOwned(T *t = nullptr)
      : m_ptr(t)
  {
    if (m_ptr)
    {
      reinterpret_cast<iObject *>(m_ptr)->AddRef();
    }
  }

  template<typename O>
  csOwned(O *o = nullptr)
      : m_ptr(static_cast<T *>(o))
  {
    if (m_ptr)
    {
      reinterpret_cast<iObject *>(m_ptr)->AddRef();
    }
  }

  csOwned(const csOwned &other)
  {
    if (other.m_ptr != m_ptr)
    {
      if (other.m_ptr)
      {
        reinterpret_cast<iObject *>(other.m_ptr)->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<iObject *>(m_ptr)->Release();
      }
      m_ptr = other.m_ptr;
    }
  }

  template<typename O>
  csOwned(const csOwned<O> &other)
  {
    O *otherPtr = other.Data();
    if (otherPtr != m_ptr)
    {
      if (otherPtr)
      {
        reinterpret_cast<iObject *>(otherPtr)->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<iObject *>(m_ptr)->Release();
      }
      m_ptr = static_cast<T *>(otherPtr);
    }
  }

  ~csOwned()
  {
    if (m_ptr)
    {
      reinterpret_cast<iObject *>(m_ptr)->Release();
      m_ptr = nullptr;
    }
  }

  csOwned &operator=(const csOwned &other)
  {
    if (this == &other)
    {
      return *this;
    }

    if (other.m_ptr != m_ptr)
    {
      if (other.m_ptr)
      {
        reinterpret_cast<iObject *>(other.m_ptr)->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<iObject *>(m_ptr)->Release();
      }
      m_ptr = other.m_ptr;
    }
    return *this;
  }

  template<typename O>
  csOwned &operator=(const csOwned<O> &other)
  {
    if (this == &other)
    {
      return *this;
    }

    O *otherPtr = other.Data();
    if (otherPtr != m_ptr)
    {
      if (otherPtr)
      {
        reinterpret_cast<iObject *>(otherPtr)->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<iObject *>(m_ptr)->Release();
      }
      m_ptr = static_cast<T *>(otherPtr);
    }
    return *this;
  }

  operator bool() const
  {
    return m_ptr != nullptr;
  }

  T *operator->() const
  {
    return m_ptr;
  }

  T *Data() const
  {
    return m_ptr;
  }

  T *Consume()
  {
    T *t = m_ptr;
    m_ptr = nullptr;
    return t;
  }

  operator csRef<T>()
  {
    return csRef<T>(m_ptr);
  };

  operator csAssetRef<T>()
  {
    return csAssetRef<T>(m_ptr);
  };

  template<typename O>
  csOwned<O> Query() const
  {
    return csOwned<O>(reinterpret_cast<iObject *>(m_ptr)->Query<O>());
  }

private:
  T *m_ptr = nullptr;
};

}