

#include <spcCore/resource/assetmanager.hh>
#include <spcCore/resource/ifile.hh>
#include <spcCore/resource/vfs.hh>
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


void AssetManager::RegisterLoader(iAssetLoaderSpc* loader)
{
  if (!loader)
  {
    return;
  }
  if (std::find(m_loadersSpc.begin(), m_loadersSpc.end(), loader) != m_loadersSpc.end())
  {
    return;
  }

  loader->AddRef();
  m_loadersSpc.push_back(loader);

  std::sort(m_loadersSpc.begin(), m_loadersSpc.end(), [](iAssetLoaderSpc* l0, iAssetLoaderSpc* l1) {
    return l0->Priority() > l1->Priority();
    });
}

iObject* AssetManager::Get(const Class* cls, const ResourceLocator& locator)
{
  return Load(cls, locator);
}


iObject* AssetManager::Load(const Class* cls, const ResourceLocator& locator)
{
  iObject* result = nullptr;
  if (locator.GetExtension() == std::string("SPC"))
  {
    result = LoadSpc(cls, locator);
  }

  if (!result)
  {
    result = LoadExt(cls, locator);
  }

  return result;
}


iObject* AssetManager::LoadExt(const Class* cls, const ResourceLocator& locator)
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


iObject* AssetManager::LoadSpc(const Class* cls, const ResourceLocator& locator)
{
  iFile* fd = spc::VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  AutoRelease arFD(fd);

  file::File file;
  if (!file.Parse(fd))
  {
    return nullptr;
  }


  for (iAssetLoaderSpc* loader : m_loadersSpc)
  {
    if (loader->CanLoad(cls, &file, &locator))
    {
      iObject* obj = loader->Load(cls, &file, &locator);
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