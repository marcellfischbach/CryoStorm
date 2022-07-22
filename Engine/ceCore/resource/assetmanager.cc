

#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/ifile.hh>
#include <ceCore/resource/vfs.hh>
#include <algorithm>

namespace ce
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
      fprintf(stderr, "Loader '%s' cannot load '%s'\n",
              loader->GetClass()->GetName().c_str(),
              locator.Encoded().c_str()
      );
    }
  }

  fprintf(stderr, "No loader found for '%s'\n",
          locator.Encoded().c_str()
  );

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