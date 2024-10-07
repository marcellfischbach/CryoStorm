#include <csCore/resource/csJavaFile.hh>
#include <csCore/csJava.hh>
#include <csCore/csJavaCalls.hh>

namespace cs
{

static std::string s_name = "";

const std::string &csJavaFile::GetName() const
{
  static csJavaCallObject<jstring>
          getName(csJava::Get(), this, "org/crimsonedge/core/resource/JavaFile", "getName", JAVA_STRING);
  jstring res = getName.call(csJava::Get(), 0);
  if (res)
  {
    m_name = csJava::Get()->GetStringUTFChars(res, 0);
  }
  return m_name;
}
bool csJavaFile::Open(eAccessMode accessMode, eOpenMode openMode)
{
  static csJavaCallBoolean2<jint, jint>
      open(csJava::Get(), this, "org/crimsonedge/core/resource/JavaFile", "open", JAVA_INT, JAVA_INT);
  return open.call(csJava::Get(), accessMode, openMode, false);
}
bool csJavaFile::IsOpen() const
{
  static csJavaCallBoolean isOpen(csJava::Get(), this, "org/crimsonedge/core/resource/JavaFile", "isOpen");
  return isOpen.call(csJava::Get(), false);
}

bool csJavaFile::IsEOF() const
{
  static csJavaCallBoolean isOpen(csJava::Get(), this, "org/crimsonedge/core/resource/JavaFile", "isEOF");
  return isOpen.call(csJava::Get(), false);

}
void csJavaFile::Seek(eSeekMode mode, long offset)
{
  static csJavaCallVoid2<jint, jlong>
      seek(csJava::Get(), this, "org/crimsonedge/core/resource/JavaFile", "seek", JAVA_INT, JAVA_LONG);
  seek.call(csJava::Get(), mode, offset);
}


long csJavaFile::Tell() const
{
  static csJavaCallLong tell(csJava::Get(), this, "org/crimsonedge/core/resource/JavaFile", "tell");
  return (long) tell.call(csJava::Get(), 0);
}

Size csJavaFile::Read(Size elementSize, Size elementCount, void *buffer)
{
  size_t     size   = elementSize * elementCount;
  JNIEnv     *pEnv  = csJava::Get();
  jbyteArray pArray = pEnv->NewByteArray((jsize) size);

  static csJavaCallInt3<jint, jint, jbyteArray> read(pEnv,
                                                     this,
                                                     "org/crimsonedge/core/resource/JavaFile",
                                                     "read",
                                                     JAVA_INT,
                                                     JAVA_INT,
                                                     JAVA_BYTE_ARRAY);

  jint readBytes = read.call(pEnv, elementSize, elementCount, pArray, 0);

  jbyte *bytes = pEnv->GetByteArrayElements(pArray, 0);
  memcpy(buffer, bytes, readBytes);
  pEnv->ReleaseByteArrayElements(pArray, bytes, 0);


  pEnv->DeleteLocalRef(pArray);

  return readBytes;
}
Size csJavaFile::Write(Size elementSize, Size elementCount, const void *buffer)
{
  return 0;
}
void csJavaFile::Close()
{
  static csJavaCallVoid close(csJava::Get(), this, "org/crimsonedge/core/resource/JavaFile", "close");
  close.call(csJava::Get());
}

}