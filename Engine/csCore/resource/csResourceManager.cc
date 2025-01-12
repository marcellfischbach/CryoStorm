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


void csResourceManager::AddResource(const cs::csResourceLocator &locator, csRes<cs::iResource> &resource)
{
  auto it = m_resources.find(locator);
  if (it != m_resources.end())
  {
    it->second->Invalidate();
    m_resources.erase(it);
  }

  m_resources[locator] = resource;
}

csRes<iResource> csResourceManager::GetResource(const cs::csResourceLocator &locator)
{
  auto it =  m_resources.find(locator);
  if (it != m_resources.end())
  {
    return it->second;
  }

  csRes <iResource> res = Load(locator);
  m_resources[locator] = res;
  return res;
}

csRes<iResource> csResourceManager::Load(const cs::csResourceLocator &locator) const
{
  return csRes<iResource>(nullptr);
}


} // cs