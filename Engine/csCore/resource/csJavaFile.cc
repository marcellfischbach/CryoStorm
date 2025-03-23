#include <csCore/resource/csJavaFile.hh>
#include <csCore/csJava.hh>
#include <csCore/csJavaCalls.hh>

namespace cs
{

#define THIS_JAVA_CLASS "org/cryo/core/resource/CsJavaFile"

static std::string s_name = "";

struct csJavaFilePrivate
{

  csJavaFilePrivate(csJavaFile *file)
  {

    JNIEnv *pEnv = csJava::Get();
    getName = csJavaCallObject<jstring>(pEnv, file, THIS_JAVA_CLASS, "getName", JAVA_STRING);
    open    = csJavaCallBoolean2<jint, jint>(pEnv, file, THIS_JAVA_CLASS, "open", JAVA_INT, JAVA_INT);
    isOpen  = csJavaCallBoolean(pEnv, file, THIS_JAVA_CLASS, "isOpen");
    isEOF   = csJavaCallBoolean(pEnv, file, THIS_JAVA_CLASS, "isEOF");
    seek    = csJavaCallVoid2<jint, jlong>(pEnv, file, THIS_JAVA_CLASS, "seek", JAVA_INT, JAVA_LONG);
    tell    = csJavaCallLong(pEnv, file, THIS_JAVA_CLASS, "tell");
    read    = csJavaCallInt3<jint, jint, jbyteArray>(pEnv,
                                                     file,
                                                     THIS_JAVA_CLASS,
                                                     "read",
                                                     JAVA_INT,
                                                     JAVA_INT,
                                                     JAVA_BYTE_ARRAY);
    close   = csJavaCallVoid(pEnv, file, THIS_JAVA_CLASS, "close");
  }

  csJavaCallObject<jstring>              getName;
  csJavaCallBoolean2<jint, jint>         open;
  csJavaCallBoolean                      isOpen;
  csJavaCallBoolean                      isEOF;
  csJavaCallVoid2<jint, jlong>           seek;
  csJavaCallLong                         tell;
  csJavaCallInt3<jint, jint, jbyteArray> read;
  csJavaCallVoid                         close;

};


csJavaFile::csJavaFile()
    : m_priv(new csJavaFilePrivate(this))
{

}

csJavaFile::~csJavaFile()
{
  delete m_priv;
  m_priv = nullptr;
}


const std::string &csJavaFile::GetName() const
{
  jstring res = m_priv->getName.call(csJava::Get(), 0);
  if (res)
  {
    m_name = csJava::Get()->GetStringUTFChars(res, 0);
  }
  return m_name;
}
bool csJavaFile::Open(eAccessMode accessMode, eOpenMode openMode)
{
  return m_priv->open.call(csJava::Get(), accessMode, openMode, false);
}
bool csJavaFile::IsOpen() const
{
  return m_priv->isOpen.call(csJava::Get(), false);
}

bool csJavaFile::IsEOF() const
{
  return m_priv->isEOF.call(csJava::Get(), false);

}
void csJavaFile::Seek(eSeekMode mode, long offset)
{
  m_priv->seek.call(csJava::Get(), mode, offset);
}


long csJavaFile::Tell() const
{
  return (long) m_priv->tell.call(csJava::Get(), 0);
}

Size csJavaFile::Read(Size elementSize, Size elementCount, void *buffer)
{
  size_t     size   = elementSize * elementCount;
  JNIEnv     *pEnv  = csJava::Get();
  jbyteArray pArray = pEnv->NewByteArray((jsize) size);



  jint readBytes = m_priv->read.call(pEnv, elementSize, elementCount, pArray, 0);

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
  m_priv->close.call(csJava::Get());
}

}