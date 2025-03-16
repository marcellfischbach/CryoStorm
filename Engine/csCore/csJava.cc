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

void csJavaThrowNoClassDefFoundError(JNIEnv *env, const char *message )
{
  const char *className = "java/lang/NoClassDefFoundError";
  jclass exClass = env->FindClass( className);
  if (!exClass) {
    exit(-1);
    return;
  }

  env->ThrowNew(exClass, message );
}


void csJavaThrowRuntimeException(JNIEnv *env, const char *message )
{
  const char *className = "java/lang/RuntimeException";
  jclass exClass = env->FindClass( className);
  if (!exClass) {
    exit(-1);
    return;
  }

  env->ThrowNew(exClass, message );
}


void csJavaThrowNoCsClassFoundException(JNIEnv *env, const char *message )
{
  const char *className = "org/cryo/core/NoCsClassFoundException";
  jclass exClass = env->FindClass( className);
  if (!exClass) {
    csJavaThrowNoClassDefFoundError(env, className);
    return;
  }

  env->ThrowNew(exClass, message );
}


void csJAvaThrowCsClassInstantiationException(JNIEnv *env, const char *message )
{
  const char *className = "org/cryo/core/CsClassInstantiationException";
  jclass exClass = env->FindClass( className);
  if (!exClass) {
    csJavaThrowNoClassDefFoundError(env, className);
    return;
  }

  env->ThrowNew(exClass, message );
}

JNIEXPORT void
JNICALL Java_org_cryo_core_CsObject_nReleaseObject(JNIEnv *env, jclass cls, jlong ref)
{
  const cs::iObject* obj = reinterpret_cast<const cs::iObject*> (ref);
  if (obj)
  {
    obj->ReleaseJObject();
  }
}


JNIEXPORT jlong
JNICALL Java_org_cryo_core_CsObject_nCreateClass(JNIEnv *env, jclass cls, jobject coreObject, jstring classNameStr)
{
  try
  {
    const char          *classNameChars = env->GetStringUTFChars(classNameStr, nullptr);
    std::string className (classNameChars);
    printf ("Create className: %s\n", className.c_str()); fflush(stdout);
    env->ReleaseStringUTFChars(classNameStr, classNameChars);

    const cs::csClass *pClass         = cs::csClassRegistry::Get()->GetClass(className);
    printf ("pClass: %p\n", pClass); fflush(stdout);
    if (!pClass)
    {
      csJavaThrowNoCsClassFoundException(env, className.c_str());
      return 0;
    }
    cs::iObject *obj = pClass->CreateInstance();
    printf ("Object: %p\n", obj); fflush(stdout);
    if (!obj)
    {
      csJAvaThrowCsClassInstantiationException(env, className.c_str());
      return 0;
    }

    obj->SetJObject(coreObject, cs::iObject::eMM_Weak);
    return reinterpret_cast<jlong>(obj);
  }
  catch (std::exception &e)
  {
    csJavaThrowRuntimeException(env, e.what());
    return 0;
  }
}

}

