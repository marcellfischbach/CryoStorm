
#include <csCore/csJava.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/csClassRegistry.hh>


extern "C"
{


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_ObjectRegistry_nGet(JNIEnv *env, jclass, jstring clsName)
{
  const char* clsString = env->GetStringUTFChars(clsName, 0);
  const cryo::csClass *cls = cryo::csClassRegistry::Get()->GetClass(clsString);
  env->ReleaseStringUTFChars(clsName, clsString);

  cryo::iObject* obj = cryo::csObjectRegistry::Get(cls);
  return obj ? obj->GetJObject() : nullptr;
}

}