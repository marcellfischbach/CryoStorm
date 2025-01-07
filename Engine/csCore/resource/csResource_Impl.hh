
namespace cs
{
//
//template<typename T>
//csResource<T>::csResource(T *res)
//    : m_res(res)
//{
//  if (m_res)
//  {
//    m_res->AddRef();
//    m_locator = m_res->GetLocator();
//  }
//}
//
//template<typename T>
//csResource<T>::~csResource()
//{
//  if (m_res)
//  {
//    if (m_res->RefCount() == 1)
//    {
//      printf("Delete: %p\n", m_res);
//      fflush(stdout);
//    }
//
//    m_res->Release();
//  }
//}
//
//
//template<typename T>
//csResource<T> &csResource<T>::operator=(csResource<T> &res)
//{
//  m_locator = res.locator();
//  CS_SET(m_res, res.raw());
//  return *this;
//}
//
//template<typename T>
//const csResource<T> &csResource<T>::operator=(const csResource<T> &res) const
//{
//  m_locator = res.locator();
//  T *tRes = const_cast<T *>(m_res);
//  const T*  resRaw = res.raw();
//  T *rRes = const_cast<T *>(resRaw);
//
//  CS_SET(tRes, rRes);
//  m_res = tRes;
//  return *this;
//}


}