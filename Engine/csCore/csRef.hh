
#pragma once


namespace cs
{



template<typename T>
class csRef
{
public:
  csRef(T* t = nullptr)
    : m_ptr(t)
  {
    if (m_ptr)
    {
      m_ptr->AddRef();
    }
  }

  csRef(const csRef<T>& other)
    : m_ptr(other.m_ptr)
  {
    if (m_ptr)
    {
      m_ptr->AddRef();
    }
  }

  template<typename O>
  csRef(const csRef<O>& other)
    : m_ptr(const_cast<csRef<O>&>(other).raw())
  {
    if (m_ptr)
    {
      m_ptr->AddRef();
    }
  }

  ~csRef()
  {
    if (m_ptr)
    {
      m_ptr->Release();
      m_ptr = nullptr;
    }
  }


  csRef<T>& operator=(T* t)
  {
    if (t)
    {
      t->AddRef();
    }
    if (m_ptr)
    {
      m_ptr->Release();
    }
    m_ptr = t;
    return *this;
  }

  csRef<T>& operator=(csRef<T> &t)
  {
    return *this = t.raw();
  }

  csRef<T>& operator=(const csRef<T>& t)
  {
    return *this = const_cast<csRef<T>&>(t).raw();
  }

  T* operator->()
  {
    return m_ptr;
  }

  const T* operator->() const
  {
    return m_ptr;
  }

  T& operator*()
  {
    return *m_ptr;
  }

  const T& operator*() const
  {
    return *m_ptr;
  }

  T* raw() 
  {
    return m_ptr;
  }

  const T* raw() const
  {
    return m_ptr;
  }


private:
  T* m_ptr;

};

}

