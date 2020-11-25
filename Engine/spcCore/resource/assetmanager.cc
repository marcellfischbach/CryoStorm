

#include <spcCore/resource/assetmanager.hh>
#include <algorithm>

namespace spc
{

AssetManager* AssetManager::s_assetManager = nullptr;

AssetManager::AssetManager()
{

}


void AssetManager::RegisterLoader(iAssetLoader* loader)
{
  if (!loader)
  {
    return;
  }
  if (std::find(m_loaders.begin(), m_loaders.end(), loader) != m_loaders.end())
  {
    return;
  }

  loader->AddRef();
  m_loaders.push_back(loader);

  std::sort(m_loaders.begin(), m_loaders.end(), [](iAssetLoader* l0, iAssetLoader* l1) {
    return l0->Priority() > l1->Priority();
    });
}

iObject* AssetManager::Get(const Class* cls, const ResourceLocator& locator)
{
  return Load(cls, locator);
}


iObject* AssetManager::Load(const Class* cls, const ResourceLocator& locator)
{
  for (iAssetLoader* loader : m_loaders)
  {
    if (loader->CanLoad(cls, locator))
    {
      iObject* obj = loader->Load(cls, locator);
      if (obj)
      {
        return obj;
      }
    }
  }
  return nullptr;
}

void AssetManager::Set(AssetManager* assetManager)
{
  s_assetManager = assetManager;
}

AssetManager* AssetManager::Get()
{
  return s_assetManager;
}


}