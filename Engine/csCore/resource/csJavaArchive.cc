//
// Created by Marcell on 11.05.2024.
//

#include <csCore/resource/csJavaArchive.hh>
#include <csCore/csJava.hh>
#include <csCore/csJavaCalls.hh>

#define THIS_JAVA_CLASS "org/crimsonedge/core/resource/JavaArchive"

namespace cryo
{


csJavaArchive::csJavaArchive(const std::string &name, const std::string &rootPath, int priority)
{
  m_name = name;
  SetRootPath(rootPath);
  SetPriority(priority);
}

const std::string& csJavaArchive::GetName() const
{
  return m_name;
}

void csJavaArchive::SetRootPath(const std::string &rootPath)
{
  static csJavaCallVoid1<jstring> setRootPath(csJava::Get(), this, THIS_JAVA_CLASS, "setRootPath", JAVA_STRING);
  setRootPath.call(csJava::Get(), csJava::Get()->NewStringUTF(rootPath.c_str()));
}

int csJavaArchive::GetPriority() const
{
  static csJavaCallInt getPriority(csJava::Get(), this, THIS_JAVA_CLASS, "getPriority");
  return getPriority.call(csJava::Get(), 0);
}

void csJavaArchive::SetPriority(int priority)
{
  static csJavaCallVoid1<jint> setPriority(csJava::Get(), this, THIS_JAVA_CLASS, "setPriority", JAVA_INT);
  setPriority.call(csJava::Get(), priority);
}

#define IF_NULL(obj) if (!(obj)) return nullptr

iFile *csJavaArchive::Open(const std::string &locator, eAccessMode accessMode, eOpenMode openMode)
{
  JNIEnv                                                 *env = csJava::Get();
  static csJavaCallObject3<jobject, jstring, jint, jint> open(env,
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

