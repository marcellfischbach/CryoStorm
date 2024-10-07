#include <csCore/resource/csAssetManager.hh>
#include <csCore/csClassRegistry.hh>

extern "C"
{

JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_resource_AssetManager_nGet(JNIEnv *env, jclass, jstring cls, jstring locator)
{
  jobject result = nullptr;
  const char* clsChar = env->GetStringUTFChars(cls, 0);
  const char* locatorChar = env->GetStringUTFChars(locator, 0);
  const cs::csClass *pClass = cs::csClassRegistry::Get()->GetClass(clsChar);
  if (pClass)
  {
    cs::iObject *pObject = cs::csAssetManager::Get()->Get(pClass, locatorChar);
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
  const cs::csClass *pClass = cs::csClassRegistry::Get()->GetClass(clsChar);
  if (pClass)
  {
    cs::iObject *pObject = cs::csAssetManager::Get()->Load(pClass, locatorChar);
    result = pObject ? pObject->GetJObject() : nullptr;
  }

  env->ReleaseStringUTFChars(cls, clsChar);
  env->ReleaseStringUTFChars(locator, locatorChar);
  return result;
}

}