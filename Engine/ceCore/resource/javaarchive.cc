//
// Created by Marcell on 11.05.2024.
//

#include <ceCore/resource/javaarchive.hh>
#include <ceCore/java.hh>

#define THIS_JAVA_CLASS "org/crimsonedge/core/resource/JavaArchive"

namespace ce
{


JavaArchive::JavaArchive(const std::string &rootPath, int priority)
{
  SetRootPath(rootPath);
  SetPriority(priority);
}

void JavaArchive::SetRootPath(const std::string &rootPath)
{
  JNIEnv        *env = Java::Get();
  CE_JCALL_VOID_STRING(env, THIS_JAVA_CLASS, "setRootPath", rootPath.c_str());
}

int JavaArchive::GetPriority() const
{
  JNIEnv  *env = Java::Get();
  jint  res = 0;
  CE_JCALL_INT(env, THIS_JAVA_CLASS, "getPriority", res, 0);
  return (int)res;
}

void JavaArchive::SetPriority(int priority)
{
  JNIEnv  *env = Java::Get();
  CE_JCALL_VOID_INT(env, "org/crimsonedge/core/resource/JavaArchive", "setPriority", priority);
}

#define IF_NULL(obj) if (!(obj)) return nullptr

iFile *JavaArchive::Open(const std::string &locator, eAccessMode accessMode, eOpenMode openMode)
{
  JNIEnv        *env = Java::Get();
  static jclass cls  = env->FindClass("org/crimsonedge/core/resource/JavaArchive");
  IF_NULL(cls);

  static jmethodID openMethod = env->GetMethodID(cls, "open", "(Ljava/lang/String;II)Lorg/crimsonedge/core/resource/IFile;");
  IF_NULL(openMethod);

  jobject javaArchive = GetJObject();
  IF_NULL(javaArchive);


  jstring str = env->NewStringUTF((locator).c_str());
  jobject res = env->CallObjectMethod(javaArchive, openMethod, str, (jint) accessMode, (jint) openMode);
  IF_NULL(res);

  static jclass fileObject = env->FindClass("org/crimsonedge/core/resource/CoreObject");
  IF_NULL(fileObject);

  static jmethodID getRefMethod = env->GetMethodID(fileObject, "getRef", "()J");
  IF_NULL(getRefMethod);

  jlong ref = env->CallLongMethod(res, getRefMethod);
  IF_NULL(ref);

  return reinterpret_cast<iFile *>(ref);
}


}

