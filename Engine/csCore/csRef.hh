
#pragma once

#include <csCore/csClass.hh>
#include <csCore/resource/iResource.hh>
#include <csCore/resource/csResourcePool.hh>

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

  csRef()
      : m_ptr(nullptr)
  {

  }

  csRef(T *t)
      : m_ptr(t)
  {
    if (m_ptr)
    {
      reinterpret_cast<iObject *>(m_ptr)->AddRef();
    }
  }

  csRef(csRef &other)
      : m_ptr(nullptr)
  {
    m_ptr = other.m_ptr;
    if (m_ptr)
    {
      reinterpret_cast<iObject *>(m_ptr)->AddRef();
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

  csRef(csRef &&other) noexcept
      : m_ptr(other.m_ptr)
  {
    other.m_ptr = nullptr;
  }

  template<typename O>
  explicit
  csRef(const csRef<O> &other)
      : m_ptr(other.m_ptr)
  {
    if (m_ptr)
    {
      reinterpret_cast<iObject *>(m_ptr)->AddRef();
    }
  }

  template<typename O>
  explicit
  csRef(csRef<O> &&other)
      : m_ptr(other.m_ptr)
  {
    other.m_ptr = nullptr;
  }


  ~csRef()
  {

    if (m_ptr)
    {
      reinterpret_cast<iObject *>(m_ptr)->Release();
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
      reinterpret_cast<iObject *>(m_ptr)->Release();
    }
    m_ptr = t;
    return *this;
  }


  csRef &operator=(csRef &&ref) noexcept
  {
    if (this != &ref)
    {
      m_ptr = ref.raw();
      ref.m_ptr = nullptr;
    }
    return *this;
  }

  csRef &operator=(const csRef &ref)
  {
    if (this != &ref)
    {
      void *t = const_cast<void *>(reinterpret_cast<const void *>(ref.raw()));
      if (t)
      {
        reinterpret_cast<iObject *>(t)->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<iObject *>(m_ptr)->Release();
      }
      m_ptr = reinterpret_cast<T *>(t);
    }
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

  const T &operator*() const
  {
    return *reinterpret_cast<const T *>(m_ptr);
  }

  operator bool() const
  {
    return m_ptr != nullptr;
  }

  bool operator==(const csRef &ref) const
  {
    return m_ptr == ref.m_ptr;
  }

  bool operator==(const T *ptr) const
  {
    return m_ptr == ptr;
  }

  T *raw()
  {
    return reinterpret_cast<T *>(m_ptr);
  }

  [[maybe_unused]] const T *raw() const
  {
    return reinterpret_cast<const T *>(m_ptr);
  }

  template<typename O>
  operator csRef<O> &()
  {
    static_cast<O *> (m_ptr);
    return *reinterpret_cast<csRef<O> *>(this);
  }

  template<typename O>
  operator const csRef<O> &() const
  {
    static_cast<O *> (m_ptr);
    return *reinterpret_cast<const csRef<O> *>(this);
  }

private:
  T *m_ptr;

};


template<typename T>
class csRes
{
public:
  csRes()
      : m_ptr(nullptr)
  {

  }

  csRes(csRef<T> &other)
      : m_ptr(other.raw())
  {
    if (m_ptr)
    {
      reinterpret_cast<iResource *>(m_ptr)->AddRef();
    }
  }

  csRes(T *t)
      : m_ptr(t)
  {
    if (m_ptr)
    {
      reinterpret_cast<iResource *>(m_ptr)->AddRef();
    }
  }

  csRes(const csRes &other)
      : m_ptr(nullptr)
  {
    m_ptr = other.m_ptr;
    if (m_ptr)
    {
      reinterpret_cast<iResource *>(m_ptr)->AddRef();
    }
  }

  csRes(csRes &&other) noexcept
      : m_ptr(other.m_ptr)
  {
    other.m_ptr = nullptr;
  }

  template<typename O>
  explicit
  csRes(const csRes<O> &other)
      : m_ptr(other.m_ptr)
  {
    if (m_ptr)
    {
      reinterpret_cast<iResource *>(m_ptr)->AddRef();
    }
  }

  template<typename O>
  explicit
  csRes(csRes<O> &&other)
      : m_ptr(other.m_ptr)
  {
    other.m_ptr = nullptr;
  }


  ~csRes()
  {

    if (m_ptr)
    {
      reinterpret_cast<iResource *>(m_ptr)->Release();
      m_ptr = nullptr;
    }
  }

  operator csRef<T>()
  {
    return csRef<T>(m_ptr);
  }

  operator const csRef<T>() const
  {
    return csRef<T>(m_ptr);
  }


  csRes &operator=(T *t)
  {
    if (t)
    {
      t->AddRef();
    }
    if (m_ptr)
    {
      reinterpret_cast<iResource *>(m_ptr)->Release();
    }
    m_ptr = t;
    return *this;
  }


  csRes &operator=(csRes &&ref) noexcept
  {
    if (this != &ref)
    {
      m_ptr = ref.raw();
      ref.m_ptr = nullptr;
    }
    return *this;
  }

  csRes &operator=(csRes &ref)
  {
    if (this != &ref)
    {
      void *t = ref.raw();
      if (t)
      {
        reinterpret_cast<iResource *>(t)->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<iResource *>(m_ptr)->Release();
      }
      m_ptr = reinterpret_cast<T *>(t);
    }
    return *this;
  }

  csRes &operator=(const csRes &ref)
  {
    if (this != &ref)
    {
      void *t = const_cast<T *>(ref.raw());
      if (t)
      {
        reinterpret_cast<iResource *>(t)->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<iResource *>(m_ptr)->Release();
      }
      m_ptr = reinterpret_cast<T *>(t);
    }
    return *this;
  }

  T *operator->()
  {
    validate();
    return reinterpret_cast<T *>(m_ptr);
  }

  const T *operator->() const
  {
    validate();
    return reinterpret_cast<const T *>(m_ptr);
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

  bool operator==(const csRes &res) const
  {
    return m_ptr == res.m_ptr;
  }

  T *raw()
  {
    validate();
    return reinterpret_cast<T *>(m_ptr);
  }

  [[maybe_unused]] const T *raw() const
  {
    validate();
    return reinterpret_cast<const T *>(m_ptr);
  }

  template<typename O>
  operator csRes<O> &()
  {
    static_cast<O *> (m_ptr);
    return *reinterpret_cast<csRes<O> *>(this);
  }

  template<typename O>
  operator const csRes<O> &() const
  {
    static_cast<O *> (m_ptr);
    return *reinterpret_cast<const csRes<O> *>(this);
  }

private:

  CS_FORCEINLINE void validate() const
  {
    if (m_ptr && !reinterpret_cast<iResource *>(m_ptr)->IsValid())
    {
      iResource *t = csResourcePool::Instance().Get(reinterpret_cast<iResource *>(m_ptr)->GetLocator());
      if (t)
      {
        t->AddRef();
      }
      if (m_ptr)
      {
        reinterpret_cast<iResource *>(m_ptr)->Release();
      }
      m_ptr = reinterpret_cast<T *>(t);
    }
  }

  mutable T *m_ptr;

};


}

