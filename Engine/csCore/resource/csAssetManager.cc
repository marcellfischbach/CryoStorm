

#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csAssetPool.hh>
#include <csCore/resource/iFile.hh>
#include <csCore/resource/iAsset.hh>
#include <csCore/resource/csVFS.hh>
#include <algorithm>

namespace cs
{

csAssetManager *csAssetManager::s_assetManager = nullptr;

csAssetManager::csAssetManager()
{

}


csAssetManager *csAssetManager::RegisterLoader(csAssetLoader *loader)
{

  m_assetLoaders.emplace_back(loader);
  std::sort(m_assetLoaders.begin(), m_assetLoaders.end(), [](csAssetLoader *l0, csAssetLoader *l1) {
    return l0->GetPriority() > l1->GetPriority();
  });
  return this;
}


csOwned<iAsset> csAssetManager::Get(const csAssetLocator &locator)
{
  iAsset *pooled = csAssetPool::Instance().Get(locator);
  if (pooled)
  {
    return csOwned<iAsset>(pooled);
  }


  csOwned<iAsset> resource = Load(locator);
  iAsset* asset = resource.Data();
  if (asset)
  {
    asset->SetLocator(locator);
    csAssetPool::Instance().Put(asset);
  }

  return resource;
}


csOwned<iAsset> csAssetManager::Load(const csAssetLocator &locator)
{
  for (csAssetLoader *loader: m_assetLoaders)
  {
    if (loader->CanLoad(locator))
    {
      csOwned<iAsset> res = loader->Load(locator);
      if (res)
      {
        return res;
      }
      fprintf(stderr, "Loader '%s' cannot load '%s'\n",
              loader->GetClass()->GetName().c_str(),
              locator.Canonical().c_str()
      );
    }
  }

  fprintf(stderr, "No loader found for '%s'\n",
          locator.Canonical().c_str()
  );

  return csOwned<iAsset>(nullptr);
}


void csAssetManager::Set(csAssetManager *assetManager)
{
  s_assetManager = assetManager;
}

csAssetManager *csAssetManager::Get()
{
  return s_assetManager;
}


}

#ifdef CS_JAVA

#include <csCore/csJava.hh>
#include <csCore/csClassRegistry.hh>

extern "C"
{


JNIEXPORT jlong JNICALL Java_org_cryo_core_resource_AssetManager_nGet__(JNIEnv *env, jclass cls)
{
  return reinterpret_cast<jlong>(cs::csAssetManager::Get());
}


JNIEXPORT jobject JNICALL Java_org_cryo_core_resource_AssetManager_nGet__JLjava_lang_String_2
    (JNIEnv *env,
     jclass,
     jlong ref,
     jstring resourceLocator)
{
  auto assetManager = reinterpret_cast<cs::csAssetManager*>(ref);
  const char* resourceLocatorPtr = env->GetStringUTFChars(resourceLocator, nullptr);
  std::string resourceLocatorStr (resourceLocatorPtr);
  env->ReleaseStringUTFChars(resourceLocator, resourceLocatorPtr);


  cs::csRef<cs::iObject> pObject = assetManager->Get(resourceLocatorStr);
  jobject result = pObject ? pObject->GetJObject() : nullptr;
  if (result)
  {
    pObject->AddRef();
  }
  
  return result;
}


JNIEXPORT jobject JNICALL Java_org_cryo_core_resource_AssetManager_nLoad__JLjava_lang_String_2
    (JNIEnv *env,
     jclass cls,
     jlong ref,
     jstring resourceLocator)
{
  auto assetManager = reinterpret_cast<cs::csAssetManager*>(ref);
  const char* resourceLocatorPtr = env->GetStringUTFChars(resourceLocator, nullptr);
  std::string resourceLocatorStr (resourceLocatorPtr);
  env->ReleaseStringUTFChars(resourceLocator, resourceLocatorPtr);

  cs::csRef<cs::iObject> pObject = assetManager->Load(resourceLocatorStr);
  jobject result = pObject ? pObject->GetJObject() : nullptr;
  if (result)
  {
    pObject->AddRef();
  }

  return result; }

};

#endif