//
// Created by MCEL on 08.05.2024.
//

#include <ceCore/java.hh>
#include <ceCore/classregistry.hh>

namespace cryo
{

JNIEnv *Java::s_env = nullptr;


void Java::Set(JNIEnv *env)
{
  s_env = env;
}

JNIEnv *Java::Get()
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
    const cryo::Class *pClass = cryo::ClassRegistry::Get()->GetClass(classNameChars);
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
