#include <ceCore/resource/javafile.hh>
#include <ceCore/java.hh>

namespace ce
{


const std::string &JavaFile::GetName() const
{
  return <#initializer#>;
}
bool JavaFile::Open(eAccessMode accessMode, eOpenMode openMode)
{
  return false;
}
bool JavaFile::IsOpen() const
{
  return false;
}
bool JavaFile::IsEOF() const
{
  return false;
}
void JavaFile::Seek(eSeekMode mode, long offset)
{

}
long JavaFile::Tell() const
{
  return 0;
}
Size JavaFile::Read(Size elementSize, Size elementCount, void *buffer)
{
  return 0;
}
Size JavaFile::Write(Size elementSize, Size elementCount, const void *buffer)
{
  return 0;
}
void JavaFile::Close()
{

}
}