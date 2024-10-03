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
  const cryo::Class *pClass = cryo::ClassRegistry::Get()->GetClass(clsChar);
  if (pClass)
  {
    cryo::iObject *pObject = cryo::AssetManager::Get()->Get(pClass, locatorChar);
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
  const cryo::Class *pClass = cryo::ClassRegistry::Get()->GetClass(clsChar);
  if (pClass)
  {
    cryo::iObject *pObject = cryo::AssetManager::Get()->Load(pClass, locatorChar);
    result = pObject ? pObject->GetJObject() : nullptr;
  }

  env->ReleaseStringUTFChars(cls, clsChar);
  env->ReleaseStringUTFChars(locator, locatorChar);
  return result;
}

}