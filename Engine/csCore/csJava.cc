//
// Created by MCEL on 08.05.2024.
//

#include <csCore/csJava.hh>
#include <csCore/csClassRegistry.hh>

namespace cs
{

JNIEnv *csJava::s_env = nullptr;


void csJava::Set(JNIEnv *env)
{
  s_env = env;
}

JNIEnv *csJava::Get()
{
  return s_env;
}

}

extern "C"
{


JNIEXPORT jlong
JNICALL Java_org_cryo_core_CoreObject_nCreateClass(JNIEnv *env, jobject coreObject, jstring classNameStr)
{
  try
  {
    const char          *classNameChars = env->GetStringUTFChars(classNameStr, nullptr);
    const cs::csClass *pClass         = cs::csClassRegistry::Get()->GetClass(classNameChars);
    env->ReleaseStringUTFChars(classNameStr, classNameChars);

    if (!pClass)
    {
      return 0;
    }
    cs::iObject *obj = pClass->CreateInstance();
    if (!obj)
    {
      return 0;
    }

    obj->SetJObject(coreObject);
    return reinterpret_cast<jlong>(obj);
  }
  catch (std::exception &e)
  {
    return 0;
  }
}

}
