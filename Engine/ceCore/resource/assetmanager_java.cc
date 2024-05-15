#include <ceCore/resource/assetmanager.hh>
#include <ceCore/classregistry.hh>

extern "C"
{

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_resource_AssetManager_nGet(JNIEnv *env, jclass, jstring cls, jstring locator)
{
  jobject result = nullptr;
  const char* clsChar = env->GetStringUTFChars(cls, 0);
  const char* locatorChar = env->GetStringUTFChars(locator, 0);
  const ce::Class *pClass = ce::ClassRegistry::Get()->GetClass(clsChar);
  if (pClass)
  {
    ce::iObject *pObject = ce::AssetManager::Get()->Get(pClass, locatorChar);
    result = pObject ? pObject->GetJObject() : nullptr;
  }

  env->ReleaseStringUTFChars(cls, clsChar);
  env->ReleaseStringUTFChars(locator, locatorChar);
  return result;
}



JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_resource_AssetManager_nLoad(JNIEnv *env, jclass, jstring cls, jstring locator)
{
  jobject result = nullptr;
  const char* clsChar = env->GetStringUTFChars(cls, 0);
  const char* locatorChar = env->GetStringUTFChars(locator, 0);
  const ce::Class *pClass = ce::ClassRegistry::Get()->GetClass(clsChar);
  if (pClass)
  {
    ce::iObject *pObject = ce::AssetManager::Get()->Load(pClass, locatorChar);
    result = pObject ? pObject->GetJObject() : nullptr;
  }

  env->ReleaseStringUTFChars(cls, clsChar);
  env->ReleaseStringUTFChars(locator, locatorChar);
  return result;
}

}