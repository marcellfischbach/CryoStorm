//
// Created by Marcell on 11.05.2024.
//

#include <csCore/resource/csJavaArchive.hh>
#include <csCore/csJava.hh>
#include <csCore/csJavaCalls.hh>

#define THIS_JAVA_CLASS "org/cryo/core/resource/CsJavaArchive"

namespace cs
{


struct csJavaArchivePrivate
{
  csJavaArchivePrivate(csJavaArchive *archive)
  {
    JNIEnv *env = csJava::Get();
    setRootPath = csJavaCallVoid1<jstring>(env, archive, THIS_JAVA_CLASS, "setRootPath", JAVA_STRING);
    open        = csJavaCallObject3<jobject, jstring, jint, jint>(env,
                                                                  archive,
                                                                  THIS_JAVA_CLASS,
                                                                  "open",
                                                                  "Lorg/cryo/core/resource/IFile;",
                                                                  JAVA_STRING,
                                                                  JAVA_INT,
                                                                  JAVA_INT);
    getPriority = csJavaCallInt(csJava::Get(), archive, THIS_JAVA_CLASS, "getPriority");
    setPriority = csJavaCallVoid1<jint>(csJava::Get(), archive, THIS_JAVA_CLASS, "setPriority", JAVA_INT);

  }
  csJavaCallVoid1<jstring>                        setRootPath;
  csJavaCallObject3<jobject, jstring, jint, jint> open;
  csJavaCallInt                                   getPriority;
  csJavaCallVoid1<jint> setPriority;

};


csJavaArchive::csJavaArchive(const std::string &name, const std::string &rootPath, int priority)
    : m_priv(new csJavaArchivePrivate(this))
{

  m_name = name;
  SetRootPath(rootPath);
  SetPriority(priority);
}

const std::string &csJavaArchive::GetName() const
{
  return m_name;
}

void csJavaArchive::SetRootPath(const std::string &rootPath)
{
  m_priv->setRootPath.call(csJava::Get(), csJava::Get()->NewStringUTF(rootPath.c_str()));
}

int csJavaArchive::GetPriority() const
{
  return m_priv->getPriority.call(csJava::Get(), 0);
}

void csJavaArchive::SetPriority(int priority)
{
  m_priv->setPriority.call(csJava::Get(), priority);
}

#define IF_NULL(obj) if (!(obj)) return nullptr

csOwned<iFile> csJavaArchive::Open(const std::string &locator, eAccessMode accessMode, eOpenMode openMode)
{
  JNIEnv *env = csJava::Get();

  jstring arg0 = env->NewStringUTF(locator.c_str());
  jobject res  = m_priv->open.call(env, arg0, accessMode, openMode, nullptr);
  IF_NULL(res);

  static jclass coreObjectClass = env->FindClass("org/cryo/core/ICsObject");
  IF_NULL(coreObjectClass);

  static jmethodID getRefMethod = env->GetMethodID(coreObjectClass, "getRef", "()J");
  IF_NULL(getRefMethod);

  jlong ref = env->CallLongMethod(res, getRefMethod);
  IF_NULL(ref);

  return reinterpret_cast<iFile *>(ref);
}

bool csJavaArchive::IsExisting(const std::string &locator) const
{
  return false;
}


}

