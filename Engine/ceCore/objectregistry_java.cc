
#include <ceCore/java.hh>
#include <ceCore/objectregistry.hh>
#include <ceCore/classregistry.hh>


extern "C"
{


JNIEXPORT jobject
JNICALL Java_org_crimsonedge_core_ObjectRegistry_nGet(JNIEnv *env, jclass, jstring clsName)
{
  const char* clsString = env->GetStringUTFChars(clsName, 0);
  const ce::Class *cls = ce::ClassRegistry::Get()->GetClass(clsString);
  env->ReleaseStringUTFChars(clsName, clsString);

  ce::iObject* obj = ce::ObjectRegistry::Get(cls);
  return obj ? obj->GetJObject() : nullptr;
}

}