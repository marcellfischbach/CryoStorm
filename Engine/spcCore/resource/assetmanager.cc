

#include <spcCore/resource/assetmanager.hh>
#include <algorithm>

namespace spc
{

AssetManager* AssetManager::s_assetManager = nullptr;

AssetManager::AssetManager()
{

}


void AssetManager::RegisterLoader(const Class* cls, iAssetLoader* loader)
{
  if (!cls || !loader)
  {
    return;
  }
  loader->AddRef();

  std::vector<iAssetLoader*>& classLoaders = m_loaders[cls];
  classLoaders.push_back(loader);

  std::sort(classLoaders.begin(), classLoaders.end(), [](iAssetLoader* l0, iAssetLoader* l1) {
    return l0->Priority() > l1->Priority();
    });
}

iObject* AssetManager::Get(const Class* cls, const ResourceLocator& locator)
{
  return Load(cls, locator);
}


iObject* AssetManager::Load(const Class* cls, const ResourceLocator& locator)
{
  for (iAssetLoader* loader : m_loaders[cls])
  {
    if (loader->CanLoad(locator))
    {
      iObject* obj = loader->Load(locator);
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