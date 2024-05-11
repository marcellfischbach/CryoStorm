//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <jni.h>


namespace ce
{


class CE_CORE_API Java
{
public:
  static void Set(JNIEnv *env);
  static JNIEnv  *Get();

private:
  Java () = default;

  static JNIEnv *s_env;
};

}



#define CE_JCALL_VOID_STRING(env, cls, method, value) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "(Ljava/lang/String;)V"); \
  if (!jmethod) return; \
  jobject obj = GetJObject();                           \
  if (!obj) return;                                     \
  jstring str = (env)->NewStringUTF(value);             \
  (env)->CallVoidMethod(obj, jmethod, str)

#define CE_JCALL_VOID_INT(env, cls, method, value) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "(I)V"); \
  if (!jmethod) return; \
  jobject obj = GetJObject();                           \
  if (!obj) return;                                     \
  (env)->CallVoidMethod(obj, jmethod, value)



#define CE_JCALL_INT(env, cls, method, res, def) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return def;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "()I"); \
  if (!jmethod) return def; \
  jobject obj = GetJObject();                           \
  if (!obj) return def;                                     \
  res = (env)->CallIntMethod(obj, jmethod)

#define CE_JCALL_STRING(env, cls, method, res, def) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return def;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "()Ljava/lang/String;"); \
  if (!jmethod) return def; \
  jobject obj = GetJObject();                           \
  if (!obj) return def;                                     \
  res = (env)->CallObjectMethod(obj, jmethod)