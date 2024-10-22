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

void throwNoClassDefFoundError( JNIEnv *env, const char *message )
{
  const char *className = "java/lang/NoClassDefFoundError";
  jclass exClass = env->FindClass( className);
  if (!exClass) {
    exit(-1);
    return;
  }

  env->ThrowNew(exClass, message );
}


void throwRuntimeException( JNIEnv *env, const char *message )
{
  const char *className = "java/lang/RuntimeException";
  jclass exClass = env->FindClass( className);
  if (!exClass) {
    exit(-1);
    return;
  }

  env->ThrowNew(exClass, message );
}


void throwNoCsClassFoundException( JNIEnv *env, const char *message )
{
  const char *className = "org/cryo/core/NoCsClassFoundException";
  jclass exClass = env->FindClass( className);
  if (!exClass) {
    throwNoClassDefFoundError( env, className );
    return;
  }

  env->ThrowNew(exClass, message );
}


void throwCsClassInstantiationException( JNIEnv *env, const char *message )
{
  const char *className = "org/cryo/core/CsClassInstantiationException";
  jclass exClass = env->FindClass( className);
  if (!exClass) {
    throwNoClassDefFoundError( env, className );
    return;
  }

  env->ThrowNew(exClass, message );
}

JNIEXPORT jlong
JNICALL Java_org_cryo_core_CsObject_nCreateClass(JNIEnv *env, jclass cls, jobject coreObject, jstring classNameStr)
{
  try
  {
    const char          *classNameChars = env->GetStringUTFChars(classNameStr, nullptr);
    std::string className (classNameChars);
    env->ReleaseStringUTFChars(classNameStr, classNameChars);

    const cs::csClass *pClass         = cs::csClassRegistry::Get()->GetClass(className);

    if (!pClass)
    {
      throwNoCsClassFoundException(env, className.c_str());
      return 0;
    }
    cs::iObject *obj = pClass->CreateInstance();
    if (!obj)
    {
      throwCsClassInstantiationException(env, className.c_str());
      return 0;
    }

    obj->SetJObject(coreObject);
    return reinterpret_cast<jlong>(obj);
  }
  catch (std::exception &e)
  {
    throwRuntimeException(env, e.what());
    return 0;
  }
}

}

