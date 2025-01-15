
#pragma once

#include <csCore/csClass.hh>
#include <csCore/resource/iAsset.hh>
#include <csCore/resource/csAssetPool.hh>

namespace cs
{


template<typename T>
class csRef
{
public:

  csRef(T *t = nullptr)
      : m_ptr(t)
  {
    if (m_ptr)
    {
      reinterpret_cast<const iObject *>(m_ptr)->AddRef();
    }
  }


  template<typename O>
  csRef(O *o)
      : m_ptr(static_cast<T*>(o))
  {
    if (m_ptr)
    {
      reinterpret_cast<const iObject *>(m_ptr)->AddRef();
    }
  }

  csRef(const csRef &other)
      : m_ptr(nullptr)
  {
    m_ptr = other.m_ptr;
    if (m_ptr)
    {
      reinterpret_cast<iObject *>(m_ptr)->AddRef();
    }
  }

  template<typename O>
  csRef(const csRef<O> &other)
      : m_ptr(nullptr)
  {
    m_ptr = static_cast<T*>(other.raw());
    if (m_ptr)
    {
      reinterpret_cast<iObject *>(m_ptr)->AddRef();
    }
  }

  ~csRef()
  {

    if (m_ptr)
    {
      reinterpret_cast<const iObject *>(m_ptr)->Release();
      m_ptr = nullptr;
    }
  }


  csRef &operator=(T *t)
  {
    if (t)
    {
      t->AddRef();
    }
    if (m_ptr)
    {
      reinterpret_cast<const iObject *>(m_ptr)->Release();
    }
    m_ptr = t;
    return *this;
  }


  template<typename O>
  csRef &operator=(O *o)
  {
    if (o)
    {
      o->AddRef();
    }
    if (m_ptr)
    {
      reinterpret_cast<const iObject *>(m_ptr)->Release();
    }
    m_ptr = static_cast<T*>(o);
    return *this;
  }


  csRef &operator=(const csRef &other)
  {

    if (other.m_ptr)
    {
      reinterpret_cast<const iObject*>(other.m_ptr)->AddRef();
    }
    if (m_ptr)
    {
      reinterpret_cast<const iObject *>(m_ptr)->Release();
    }
    m_ptr = other.m_ptr;
    return *this;
  }


  template<typename O>
  csRef &operator=(const csRef<O> &other)
  {
    O* o = other.raw();
    if (o)
    {
      reinterpret_cast<const iObject*>(o)->AddRef();
    }
    if (m_ptr)
    {
      reinterpret_cast<const iObject *>(m_ptr)->Release();
    }
    m_ptr = static_cast<T*>(o);
    return *this;
  }

  T *operator->()
  {
    return reinterpret_cast<T *>(m_ptr);
  }

  const T *operator->() const
  {
    return reinterpret_cast<const T *>(m_ptr);
  }


  T &operator*()
  {
    return *reinterpret_cast<T *>(m_ptr);
  }

  operator bool() const
  {
    return m_ptr != nullptr;
  }


  CS_FORCEINLINE operator T *() const
  {

    return m_ptr;
  }

  T *raw() const
  {
    return reinterpret_cast<T *>(m_ptr);
  }


private:
  T *m_ptr;

};


template<typename T>
class csAssetRef
{
public:


  csAssetRef(T *t = nullptr)
      : m_ptr(t)
  {
    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->AddRef();
    }
  }

  template<typename O>
  csAssetRef(O *o)
      : m_ptr(static_cast<T*>(o))
  {
    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->AddRef();
    }
  }

  csAssetRef(const csAssetRef &other)
      : m_ptr(nullptr)
  {
    m_ptr = other.m_ptr;
    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->AddRef();
    }
  }

  template<typename O>
  csAssetRef(const csAssetRef<O> &other)
      : m_ptr(nullptr)
  {
    m_ptr = static_cast<T*>(other.raw());
    if (m_ptr)
    {
      reinterpret_cast<iAsset *>(m_ptr)->AddRef();
    }
  }

  ~csAssetRef()
  {

    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->Release();
      m_ptr = nullptr;
    }
  }



  csAssetRef &operator=(T *t)
  {
    if (t)
    {
      t->AddRef();
    }
    if (m_ptr)
    {
      reinterpret_cast<iAsset *>(m_ptr)->Release();
    }
    m_ptr = t;
    return *this;
  }

  template<typename O>
  csAssetRef &operator=(O *o)
  {
    if (o)
    {
      o->AddRef();
    }
    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->Release();
    }
    m_ptr = static_cast<T*>(o);
    return *this;
  }


  csAssetRef &operator=(const csAssetRef &other)
  {

    if (other.m_ptr)
    {
      reinterpret_cast<const iAsset*>(other.m_ptr)->AddRef();
    }
    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->Release();
    }
    m_ptr = other.m_ptr;
    return *this;
  }


  template<typename O>
  csAssetRef &operator=(const csAssetRef<O> &other)
  {
    O* o = other.raw();
    if (o)
    {
      reinterpret_cast<const iAsset*>(o)->AddRef();
    }
    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->Release();
    }
    m_ptr = static_cast<T*>(o);
    return *this;
  }

  T *operator->() const
  {
    validate();
    return reinterpret_cast<T *>(m_ptr);
  }

  T &operator*()
  {
    validate();
    return *reinterpret_cast<T *>(m_ptr);
  }

  const T &operator*() const
  {
    validate();
    return *reinterpret_cast<const T *>(m_ptr);
  }

  operator bool() const
  {
    return m_ptr != nullptr;
  }

  CS_FORCEINLINE operator T *() const
  {

    validate();
    return m_ptr;
  }


  T *raw() const
  {
    validate();
    return reinterpret_cast<T *>(m_ptr);
  }


private:

  CS_FORCEINLINE void validate() const
  {
    if (m_ptr && !reinterpret_cast<const iAsset *>(m_ptr)->IsValid())
    {
      iAsset *t = csAssetPool::Instance().Get(reinterpret_cast<const iAsset *>(m_ptr)->GetLocator());
      if (t)
      {
        t->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<const iAsset *>(m_ptr)->Release();
      }
      m_ptr = reinterpret_cast<T *>(t);
    }
  }


  mutable T *m_ptr;

};


}

