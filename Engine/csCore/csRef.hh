
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
  static csRef &Null()
  {
    static csRef ref;
    return ref;
  }


  csRef(T *t = nullptr)
      : m_ptr(t)
  {
    if (m_ptr)
    {
      reinterpret_cast<const iObject *>(m_ptr)->AddRef();
    }
  }

//  csRef(csRef &other)
//      : m_ptr(nullptr)
//  {
//    m_ptr = other.m_ptr;
//    if (m_ptr)
//    {
//      reinterpret_cast<iObject *>(m_ptr)->AddRef();
//    }
//  }
//
//  csRef(const csRef &other)
//      : m_ptr(nullptr)
//  {
//    m_ptr = other.m_ptr;
//    if (m_ptr)
//    {
//      reinterpret_cast<const iObject *>(m_ptr)->AddRef();
//    }
//  }
//
//
//  template<typename O>
//  csRef(const csRef<O> &other)
//      : m_ptr(other.raw())
//  {
//    if (m_ptr)
//    {
//      reinterpret_cast<const iObject *>(m_ptr)->AddRef();
//    }
//  }
//
//  csRef(csRef &&other) noexcept
//      : m_ptr(other.m_ptr)
//  {
//    other.m_ptr = nullptr;
//  }
//
//
//  template<typename O>
//  explicit
//  csRef(csRef<O> &&other) noexcept
//      : m_ptr(other.m_ptr)
//  {
//    other.m_ptr = nullptr;
//  }
//

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


//  csRef &operator=(csRef &&ref) noexcept
//  {
//    if (this != &ref)
//    {
//      m_ptr = ref.raw();
//      ref.m_ptr = nullptr;
//    }
//    return *this;
//  }
//
//  csRef &operator=(const csRef &ref)
//  {
//    if (this != &ref)
//    {
//      void *t = const_cast<void *>(reinterpret_cast<const void *>(ref.raw()));
//      if (t)
//      {
//        reinterpret_cast<iObject *>(t)->AddRef();
//      }
//      if (m_ptr)
//      {
//        reinterpret_cast<iObject *>(m_ptr)->Release();
//      }
//      m_ptr = reinterpret_cast<T *>(t);
//    }
//    return *this;
//  }

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
//
//  bool operator==(const csRef &ref) const
//  {
//    return m_ptr == ref.m_ptr;
//  }

  bool operator==(const T *ptr) const
  {
    return m_ptr == ptr;
  }

//  bool operator!=(const T *ptr) const
//  {
//    return m_ptr != ptr;
//  }

  operator T *()
  {
    return m_ptr;
  }

  operator const T *() const
  {
    return m_ptr;
  }

  T *raw() const
  {
    return reinterpret_cast<T *>(m_ptr);
  }


//  template<typename O>
//  operator csRef<O> &()
//  {
//    static_cast<O *> (m_ptr);
//    return *reinterpret_cast<csRef<O> *>(this);
//  }


private:
  T *m_ptr;

};


template<typename T>
class csAssetRef
{
public:

  static csAssetRef &Null()
  {
    static csAssetRef asset;
    return asset;
  }


  csAssetRef(T *t = nullptr)
      : m_ptr(t)
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


  csAssetRef(const csRef<T> &other)
      : m_ptr(other.raw())
  {
    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->AddRef();
    }
  }


  csAssetRef(csAssetRef &&other) noexcept
      : m_ptr(other.m_ptr)
  {
    other.m_ptr = nullptr;
  }

  template<typename O>
  csAssetRef(const csAssetRef<O> &other)
      : m_ptr(other.raw())
  {
    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->AddRef();
    }
  }

  template<typename O>
  csAssetRef(csAssetRef<O> &&other) noexcept
      : m_ptr(other.m_ptr)
  {
    other.m_ptr = nullptr;
  }


  ~csAssetRef()
  {

    if (m_ptr)
    {
      reinterpret_cast<const iAsset *>(m_ptr)->Release();
      m_ptr = nullptr;
    }
  }

  operator csRef<T>()
  {
    return csRef<T>(m_ptr);
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


  csAssetRef &operator=(csAssetRef &&ref) noexcept
  {
    if (this != &ref)
    {
      m_ptr = ref.raw();
      ref.m_ptr = nullptr;
    }
    return *this;
  }

  csAssetRef &operator=(const csAssetRef &ref)
  {
    if (this != &ref)
    {
      const void *t = ref.raw();
      if (t)
      {
        reinterpret_cast<const iAsset *>(t)->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<const iAsset *>(m_ptr)->Release();
      }
      m_ptr = reinterpret_cast<T *>(const_cast<void *>(t));
    }
    return *this;
  }


  T *operator->()
  {
    validate();
    return reinterpret_cast<T *>(m_ptr);
  }


  T &operator*()
  {
    validate();
    return *reinterpret_cast<T *>(m_ptr);
  }


  operator bool() const
  {
    return m_ptr != nullptr;
  }

  bool operator==(const csAssetRef &res) const
  {
    return m_ptr == res.m_ptr;
  }


  T *raw() const
  {
    validate();
    return reinterpret_cast<T *>(m_ptr);
  }

  template<typename O>
  operator csAssetRef<O> &()
  {
    static_cast<O *> (m_ptr);
    return *reinterpret_cast<csAssetRef<O> *>(this);
  }

  //
  // type conversion from 'const csAssetRef<T>&' -> 'csAssetRef<const T>&'. This is safe to do because
  // the inner const is much more restrictive than the outer const because the csAs
  operator csAssetRef<const T> &() const
  {
    return *reinterpret_cast<csAssetRef<const T> *>(const_cast<csAssetRef *>(this));
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

