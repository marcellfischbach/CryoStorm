
#pragma  once

namespace cryo
{


template<typename T>
class AutoDelete
{
public:
  AutoDelete (T* t)
  : m_t(t)
  {

  }
  ~AutoDelete()
  {
    if (m_t)
    {
      delete m_t;
    }
  }

  AutoDelete& operator=(T* t)
  {
    m_t = t;
  }

  T* operator*()
  {
    return m_t;
  }

  T& operator&()
  {
    return *m_t;
  }

  T* operator->()
  {
    return m_t;
  }


  const T& operator*() const
  {
    return *m_t;
  }

  const T* operator&() const
  {
    return m_t;
  }

  const T* operator->() const
  {
    return m_t;
  }

  operator T*()
  {
    return m_t;
  }

  operator const T*() const
  {
    return m_t;
  }


private:
  T* m_t;

};

}