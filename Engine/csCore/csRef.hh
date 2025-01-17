
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


  T *operator->() const
  {
    return reinterpret_cast<T *>(m_ptr);
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

