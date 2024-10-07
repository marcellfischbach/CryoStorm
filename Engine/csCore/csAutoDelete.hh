
#pragma  once

namespace cs
{


template<typename T>
class csAutoDelete
{
public:
  csAutoDelete(T* t)
  : m_t(t)
  {

  }
  ~csAutoDelete()
  {
    if (m_t)
    {
      delete m_t;
    }
  }

  csAutoDelete& operator=(T* t)
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