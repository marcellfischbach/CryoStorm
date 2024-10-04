//
// Created by MCEL on 08.05.2024.
//

#include <ceCore/csJava.hh>
#include <ceCore/csClassRegistry.hh>

namespace cryo
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
JNICALL Java_org_crimsonedge_core_CoreObject_nCreateClass(JNIEnv *env, jobject coreObject, jstring classNameStr)
{
  try
  {
    const char      *classNameChars = env->GetStringUTFChars(classNameStr, 0);
    const cryo::Class *pClass = cryo::csClassRegistry::Get()->GetClass(classNameChars);
    env->ReleaseStringUTFChars(classNameStr, classNameChars);

    if (!pClass)
    {
      return 0;
    }
    cryo::iObject *obj = pClass->CreateInstance();
    if (!obj)
    {
      return 0;
    }

    fflush(stdout);
    coreObject = env->NewGlobalRef(coreObject);
    obj->SetJObject(coreObject);
    return reinterpret_cast<jlong>(obj);
  }
  catch (std::exception &e)
  {
    return 0;
  }
}

}
