//
// Created by Marcell on 11.05.2024.
//

#include <ceCore/resource/javaarchive.hh>
#include <ceCore/java.hh>
#include <ceCore/java_methods.hh>

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
  static JavaCallVoid1<jstring> setRootPath(Java::Get(), this, THIS_JAVA_CLASS, "setRootPath", JAVA_STRING);
  setRootPath.call(Java::Get(), Java::Get()->NewStringUTF(rootPath.c_str()));
}

int JavaArchive::GetPriority() const
{
  static JavaCallInt getPriority(Java::Get(), this, THIS_JAVA_CLASS, "getPriority");
  return getPriority.call(Java::Get(), 0);
}

void JavaArchive::SetPriority(int priority)
{
  static JavaCallVoid1<jint> setPriority(Java::Get(), this, THIS_JAVA_CLASS, "setPriority", JAVA_INT);
  setPriority.call(Java::Get(), priority);
}

#define IF_NULL(obj) if (!(obj)) return nullptr

iFile *JavaArchive::Open(const std::string &locator, eAccessMode accessMode, eOpenMode openMode)
{
  JNIEnv                                               *env = Java::Get();
  static JavaCallObject3<jobject, jstring, jint, jint> open(env,
                                                            this,
                                                            THIS_JAVA_CLASS,
                                                            "open",
                                                            "Lorg/crimsonedge/core/resource/IFile;",
                                                            JAVA_STRING,
                                                            JAVA_INT,
                                                            JAVA_INT);

  jstring arg0 = env->NewStringUTF(locator.c_str());
  jobject res  = open.call(env, arg0, accessMode, openMode, nullptr);
  IF_NULL(res);

  static jclass coreObjectClass = env->FindClass("org/crimsonedge/core/CoreObject");
  IF_NULL(coreObjectClass);

  static jmethodID getRefMethod = env->GetMethodID(coreObjectClass, "getRef", "()J");
  IF_NULL(getRefMethod);

  jlong ref = env->CallLongMethod(res, getRefMethod);
  IF_NULL(ref);

  return reinterpret_cast<iFile *>(ref);
}


}

