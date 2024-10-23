//
// Created by MCEL on 08.05.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <jni.h>


namespace cs
{


class CS_CORE_API csJava
{
public:
  static void Set(JNIEnv *env);
  static JNIEnv *Get();

private:
  csJava() = default;

  static JNIEnv *s_env;
};

}


extern "C"
{
void csJavaThrowNoClassDefFoundError(JNIEnv *env, const char *message);
void csJavaThrowRuntimeException(JNIEnv *env, const char *message);
void csJavaThrowNoCsClassFoundException(JNIEnv *env, const char *message);
void csJAvaThrowCsClassInstantiationException(JNIEnv *env, const char *message);
}

#define CS_JCALL_VOID(env, cls, method) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "(I)V"); \
  if (!jmethod) return; \
  jobject obj = GetJObject();                           \
  if (!obj) return;                                     \
  (env)->CallVoidMethod(obj, jmethod)


#define CS_JCALL_VOID_STRING(env, cls, method, value) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "(Ljava/lang/String;)V"); \
  if (!jmethod) return; \
  jobject obj = GetJObject();                           \
  if (!obj) return;                                     \
  jstring str = (env)->NewStringUTF(value);             \
  (env)->CallVoidMethod(obj, jmethod, str)

#define CS_JCALL_VOID_INT(env, cls, method, value) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "(I)V"); \
  if (!jmethod) return; \
  jobject obj = GetJObject();                           \
  if (!obj) return;                                     \
  (env)->CallVoidMethod(obj, jmethod, value)

#define CS_JCALL_VOID_INT_LONG(env, cls, method, value0, value1) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "(IJ)V"); \
  if (!jmethod) return; \
  jobject obj = GetJObject();                           \
  if (!obj) return;                                     \
  (env)->CallVoidMethod(obj, jmethod, value0, value1)


#define CS_JCALL_INT(env, cls, method, res, def) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return def;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "()I"); \
  if (!jmethod) return def; \
  jobject obj = GetJObject();                           \
  if (!obj) return def;                                     \
  res = (env)->CallIntMethod(obj, jmethod)


#define CS_JCALL_INT_INT_INT_BYTEARRAY(env, cls, method, value0, value1, value2, res, def) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return def;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "(II[B)I"); \
  if (!jmethod) return def; \
  jobject obj = GetJObject();                           \
  if (!obj) return def;                                     \
  res = (env)->CallIntMethod(obj, jmethod, value0, value1, value2)

#define CS_JCALL_LONG(env, cls, method, res, def) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return def;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "()J"); \
  if (!jmethod) return def; \
  jobject obj = GetJObject();                           \
  if (!obj) return def;                                     \
  res = (env)->CallIntMethod(obj, jmethod)

#define CS_JCALL_STRING(env, cls, method, res, def) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return def;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "()Ljava/lang/String;"); \
  if (!jmethod) return def; \
  jobject obj = GetJObject();                           \
  if (!obj) return def;                                     \
  res = static_cast<jstring>((env)->CallObjectMethod(obj, jmethod))

#define CS_JCALL_BOOL(env, cls, method, res, def) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return def;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "()Z"); \
  if (!jmethod) return def; \
  jobject obj = GetJObject();                           \
  if (!obj) return def;                                     \
  res = (env)->CallBooleanMethod(obj, jmethod)

#define CS_JCALL_BOOL_INT_INT(env, cls, method, value0, value1, res, def) \
  static jclass jcls = (env)->FindClass(cls); \
  if (!cls) return def;                                     \
  static jmethodID jmethod = (env)->GetMethodID(jcls, method, "(II)Z"); \
  if (!jmethod) return def; \
  jobject obj = GetJObject();                           \
  if (!obj) return def;                                     \
  res = (env)->CallBooleanMethod(obj, jmethod, value0, value1)
