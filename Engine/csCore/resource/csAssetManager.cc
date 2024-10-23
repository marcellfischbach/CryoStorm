

#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/iFile.hh>
#include <csCore/resource/csVFS.hh>
#include <algorithm>

namespace cs
{

csAssetManager *csAssetManager::s_assetManager = nullptr;

csAssetManager::csAssetManager()
{

}


void csAssetManager::RegisterLoader(iAssetLoader *loader)
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

  std::sort(m_loaders.begin(), m_loaders.end(), [](iAssetLoader *l0, iAssetLoader *l1) {
    return l0->Priority() > l1->Priority();
  });
}


iObject *csAssetManager::Get(const csClass *cls, const csResourceLocator &locator)
{
  auto it = m_cachedObjects.find(locator);
  if (it != m_cachedObjects.end())
  {
    return it->second;
  }
  iObject *obj = Load(cls, locator);
  CS_ADDREF(obj);
  m_cachedObjects[locator] = obj;
  return obj;
}


iObject *csAssetManager::Load(const csClass *cls, const csResourceLocator &locator)
{
  for (iAssetLoader *loader: m_loaders)
  {
    if (loader->CanLoad(cls, locator))
    {
      iObject *obj = loader->Load(cls, locator);
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