

#include <csCore/resource/csFileSystemFile.hh>
#include <csCore/resource/csResourceExceptions.hh>

namespace cryo
{
csFileSystemFile::csFileSystemFile(const std::string& path)
  : iFile()
  , m_path(path)
  , m_file(nullptr)
{
  CS_CLASS_GEN_CONSTR;
}

const std::string &csFileSystemFile::GetName() const
{
  return m_path;
}

bool csFileSystemFile::Open(eAccessMode accessMode, eOpenMode openMode)
{
  std::string mode;
  switch (accessMode)
  {
  case eAM_Read:
    mode = "r";
    break;
  case eAM_Write:
    mode = "w";
    break;
  case eAM_WriteAppend:
    mode = "a";
    break;
  }
  switch (openMode)
  {
  case eOM_Binary:
    mode += "b";
    break;
  case eOM_Text:
    mode += "t";
    break;
  }

  fopen_s(&m_file, m_path.c_str(), mode.c_str());
  return m_file != nullptr;
}

bool csFileSystemFile::IsOpen() const
{
  return m_file != nullptr;
}

bool csFileSystemFile::IsEOF() const
{
  if (!m_file)
  {
    throw csFileNotOpenException();
  }
  return std::feof(m_file);
}

void csFileSystemFile::Seek(eSeekMode mode, long offset)
{
  if (!m_file)
  {
    throw csFileNotOpenException();
  }
  std::fseek(m_file, offset, mode);
}
long csFileSystemFile::Tell() const
{
  if (!m_file)
  {
    throw csFileNotOpenException();
  }

  return std::ftell(m_file);
}

Size csFileSystemFile::Read(Size elementSize, Size elementCount, void* buffer)
{
  if (!m_file)
  {
    throw csFileNotOpenException();
  }
  return std::fread(buffer, elementSize, elementCount, m_file);
}

Size csFileSystemFile::Write(Size elementSize, Size elementCount, const void* buffer)
{
  if (!m_file)
  {
    throw csFileNotOpenException();
  }
  return std::fwrite(buffer, elementSize, elementCount, m_file);
}

void csFileSystemFile::Close()
{
  if (m_file)
  {
    std::fclose(m_file);
    m_file = nullptr;
  }
}

FILE* csFileSystemFile::GetHandle() const
{
  return m_file;
}

}