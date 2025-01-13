

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


void csAssetManager::RegisterLoader(csAssetLoader *loader)
{

  m_assetLoaders.push_back(loader);
  std::sort(m_assetLoaders.begin(), m_assetLoaders.end(), [](csAssetLoader *l0, csAssetLoader *l1) {
    return l0->GetPriority() > l1->GetPriority();
  });
}


csAssetRef<iAsset> csAssetManager::Get(const csAssetLocator &locator)
{
  iAsset *pooled = csAssetPool::Instance().Get(locator);
  if (pooled)
  {
    return {pooled};
  }


  csAssetRef<iAsset> resource = Load(locator);
  resource->SetLocator(locator);

  return resource;
}


csAssetRef<iAsset> csAssetManager::Load(const csAssetLocator &locator)
{
  for (csAssetLoader *loader: m_assetLoaders)
  {
    if (loader->CanLoad(locator))
    {
      csAssetRef<iAsset> res = loader->Load(locator);
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

  return csAssetRef<iAsset>::Null();
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


JNIEXPORT jobject JNICALL Java_org_cryo_core_resource_AssetManager_nGet__JLjava_lang_String_2Ljava_lang_String_2
    (JNIEnv *env,
     jclass cls,
     jlong ref,
     jstring csClass,
     jstring resourceLocator)
{
  auto assetManager = reinterpret_cast<cs::csAssetManager*>(ref);
  const char* csClassPtr = env->GetStringUTFChars(csClass, nullptr);
  const char* resourceLocatorPtr = env->GetStringUTFChars(resourceLocator, nullptr);
  std::string csClassStr (csClassPtr);
  std::string resourceLocatorStr (resourceLocatorPtr);
  env->ReleaseStringUTFChars(csClass, csClassPtr);
  env->ReleaseStringUTFChars(resourceLocator, resourceLocatorPtr);

  const cs::csClass *csClassClass = cs::csClassRegistry::Get()->GetClass(csClassStr);
  if (!csClassClass)
  {
    csJavaThrowNoCsClassFoundException(env, csClassStr.c_str());
    return nullptr;
  }
  cs::iObject *pObject = assetManager->Get(csClassClass, resourceLocatorStr);
  return pObject ? pObject->GetJObject() : nullptr;
}


JNIEXPORT jobject JNICALL Java_org_cryo_core_resource_AssetManager_nLoad__JLjava_lang_String_2Ljava_lang_String_2
    (JNIEnv *env,
     jclass cls,
     jlong ref,
     jstring csClass,
     jstring resourceLocator)
{
  auto assetManager = reinterpret_cast<cs::csAssetManager*>(ref);
  const char* csClassPtr = env->GetStringUTFChars(csClass, nullptr);
  const char* resourceLocatorPtr = env->GetStringUTFChars(resourceLocator, nullptr);
  std::string csClassStr (csClassPtr);
  std::string resourceLocatorStr (resourceLocatorPtr);
  env->ReleaseStringUTFChars(csClass, csClassPtr);
  env->ReleaseStringUTFChars(resourceLocator, resourceLocatorPtr);

  const cs::csClass *csClassClass = cs::csClassRegistry::Get()->GetClass(csClassStr);
  if (!csClassClass)
  {
    csJavaThrowNoCsClassFoundException(env, csClassStr.c_str());
    return nullptr;
  }
  cs::iObject *pObject = assetManager->Load(csClassClass, resourceLocatorStr);
  return pObject ? pObject->GetJObject() : nullptr;
}

};

#endif