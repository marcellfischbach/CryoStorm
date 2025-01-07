
#include <csCore/resource/csResourcePool.hh>

namespace cs
{

csResourcePool& csResourcePool::Instance()
{
  static csResourcePool pool;
  return pool;
}

csResourcePool::csResourcePool()
{

}

iResource* csResourcePool::Get(const csResourceLocator& locator)
{
  auto it = m_resources.find(locator);
  if (it == m_resources.end())
  {
    return nullptr;
  }
  return it->second;
}

void csResourcePool::Erase(const csResourceLocator& locator)
{
  auto it = m_resources.find(locator);
  if (it != m_resources.end())
  {
    it->second->Invalidate();
    it->second->Release();
    m_resources.erase(it);
  }
}

void csResourcePool::Put(iResource* resource)
{
  if (resource)
  {
    Put(resource->GetLocator(), resource);
  }
}
void csResourcePool::Put(const csResourceLocator& locator, iResource* resource)
{
  if (!resource)
  {
    Erase(locator);
    return;
  }

  resource->AddRef();

  auto it = m_resources.find(locator);
  if (it != m_resources.end())
  {
    it->second->Invalidate();
    it->second->Release();
  }
  m_resources[locator] = resource;
}



}