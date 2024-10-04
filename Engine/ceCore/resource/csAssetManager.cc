

#include <ceCore/resource/csAssetManager.hh>
#include <ceCore/resource/iFile.hh>
#include <ceCore/resource/csVFS.hh>
#include <algorithm>

namespace cryo
{

csAssetManager * csAssetManager::s_assetManager = nullptr;

csAssetManager::csAssetManager()
{

}


void csAssetManager::RegisterLoader(iAssetLoader* loader)
{
  if (!loader)
  {
    return;
  }
  if (std::ranges::find(m_loaders.begin(), m_loaders.end(), loader) != m_loaders.end())
  {
    return;
  }

  loader->AddRef();
  m_loaders.push_back(loader);

  std::sort(m_loaders.begin(), m_loaders.end(), [](iAssetLoader* l0, iAssetLoader* l1) {
    return l0->Priority() > l1->Priority();
    });
}


iObject* csAssetManager::Get(const Class* cls, const csResourceLocator& locator)
{
  auto it = m_cachedObjects.find(locator);
  if (it != m_cachedObjects.end())
  {
    return it->second;
  }
  iObject* obj = Load(cls, locator);
  CS_ADDREF(obj);
  m_cachedObjects[locator] = obj;
  return obj;
}


iObject* csAssetManager::Load(const Class* cls, const csResourceLocator& locator)
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



void csAssetManager::Set(csAssetManager* assetManager)
{
  s_assetManager = assetManager;
}

csAssetManager* csAssetManager::Get()
{
  return s_assetManager;
}


}