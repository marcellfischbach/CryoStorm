
#include <csCore/resource/csAssetPool.hh>

namespace cs
{

csAssetPool& csAssetPool::Instance()
{
  static csAssetPool pool;
  return pool;
}

csAssetPool::csAssetPool()
{

}

iAsset* csAssetPool::Get(const csAssetLocator& locator)
{
  auto it = m_resources.find(locator);
  if (it == m_resources.end())
  {
    return nullptr;
  }
  return it->second;
}

void csAssetPool::Erase(const csAssetLocator& locator)
{
  auto it = m_resources.find(locator);
  if (it != m_resources.end())
  {
    it->second->Invalidate();
    it->second->Release();
    m_resources.erase(it);
  }
}

void csAssetPool::Put(iAsset* resource)
{
  if (resource)
  {
    Put(resource->GetLocator(), resource);
  }
}
void csAssetPool::Put(const csAssetLocator& locator, iAsset* resource)
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
    if (it->second == resource)
    {
      return;
    }
    it->second->Invalidate();
    it->second->Release();
  }
  m_resources[locator] = resource;
  resource->Revalidate();
}



}