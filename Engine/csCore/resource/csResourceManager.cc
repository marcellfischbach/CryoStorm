//
// Created by Marcell on 12.01.2025.
//

#include <csCore/resource/csResourceManager.hh>

namespace cs
{

csResourceManager &csResourceManager::Instance()
{
  static csResourceManager instance;
  return instance;
}


void csResourceManager::AddResource(const cs::csAssetLocator &locator, csAssetRef<cs::iAsset> &resource)
{
  auto it = m_resources.find(locator);
  if (it != m_resources.end())
  {
    it->second->Invalidate();
    m_resources.erase(it);
  }

  m_resources[locator] = resource;
}

csAssetRef<iAsset> csResourceManager::GetResource(const cs::csAssetLocator &locator)
{
  auto it =  m_resources.find(locator);
  if (it != m_resources.end())
  {
    return it->second;
  }

  csAssetRef <iAsset> res = Load(locator);
  m_resources[locator] = res;
  return res;
}

csAssetRef<iAsset> csResourceManager::Load(const cs::csAssetLocator &locator) const
{
  return csAssetRef<iAsset>(nullptr);
}


} // cs