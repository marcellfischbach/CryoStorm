
namespace cs
{

template<typename T>
csResource<T>::csResource(T* res)
  : m_res(res)
{
  if (m_res)
  {
    m_res->AddRef();
    m_locator = m_res->GetLocator();
  }
}

template<typename T>
csResource<T>::~csResource()
{
  if (m_res)
  {
    m_res->Release();
  }
}

}