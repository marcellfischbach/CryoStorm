#include <ceCore/resource/javafile.hh>
#include <ceCore/java.hh>
#include <ceCore/java_methods.hh>

namespace ce
{

static std::string s_name = "";

const std::string &JavaFile::GetName() const
{
  static JavaCallObject<jstring>
          getName(Java::Get(), this, "org/crimsonedge/core/resource/JavaFile", "getName", JAVA_STRING);
  jstring res = getName.call(Java::Get(), 0);
  if (res)
  {
    m_name = Java::Get()->GetStringUTFChars(res, 0);
  }
  return m_name;
}
bool JavaFile::Open(eAccessMode accessMode, eOpenMode openMode)
{
  static JavaCallBoolean2<jint, jint>
      open(Java::Get(), this, "org/crimsonedge/core/resource/JavaFile", "open", JAVA_INT, JAVA_INT);
  return open.call(Java::Get(), accessMode, openMode, false);
}
bool JavaFile::IsOpen() const
{
  static JavaCallBoolean isOpen(Java::Get(), this, "org/crimsonedge/core/resource/JavaFile", "isOpen");
  return isOpen.call(Java::Get(), false);
}

bool JavaFile::IsEOF() const
{
  static JavaCallBoolean isOpen(Java::Get(), this, "org/crimsonedge/core/resource/JavaFile", "isEOF");
  return isOpen.call(Java::Get(), false);

}
void JavaFile::Seek(eSeekMode mode, long offset)
{
  static JavaCallVoid2<jint, jlong>
      seek(Java::Get(), this, "org/crimsonedge/core/resource/JavaFile", "seek", JAVA_INT, JAVA_LONG);
  seek.call(Java::Get(), mode, offset);
}


long JavaFile::Tell() const
{
  static JavaCallLong tell(Java::Get(), this, "org/crimsonedge/core/resource/JavaFile", "tell");
  return (long) tell.call(Java::Get(), 0);
}

Size JavaFile::Read(Size elementSize, Size elementCount, void *buffer)
{
  size_t     size   = elementSize * elementCount;
  JNIEnv     *pEnv  = Java::Get();
  jbyteArray pArray = pEnv->NewByteArray((jsize) size);

  static JavaCallInt3<jint, jint, jbyteArray> read(pEnv,
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
Size JavaFile::Write(Size elementSize, Size elementCount, const void *buffer)
{
  return 0;
}
void JavaFile::Close()
{
  static JavaCallVoid close(Java::Get(), this, "org/crimsonedge/core/resource/JavaFile", "close");
  close.call(Java::Get());
}

}