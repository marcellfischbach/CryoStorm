

#include <ceCore/resource/filesystemfile.hh>
#include <ceCore/resource/resourceexceptions.hh>

namespace ce
{
FileSystemFile::FileSystemFile(const std::string& path)
  : iFile()
  , m_path(path)
  , m_file(nullptr)
{
  CE_CLASS_GEN_CONSTR;
}

const std::string &FileSystemFile::GetName() const
{
  return m_path;
}

bool FileSystemFile::Open(eAccessMode accessMode, eOpenMode openMode)
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

bool FileSystemFile::IsOpen() const
{
  return m_file != nullptr;
}

bool FileSystemFile::IsEOF() const
{
  if (!m_file)
  {
    throw FileNotOpenException();
  }
  return std::feof(m_file);
}

void FileSystemFile::Seek(eSeekMode mode, long offset)
{
  if (!m_file)
  {
    throw FileNotOpenException();
  }
  std::fseek(m_file, offset, mode);
}
long FileSystemFile::Tell() const
{
  if (!m_file)
  {
    throw FileNotOpenException();
  }

  return std::ftell(m_file);
}

Size FileSystemFile::Read(Size elementSize, Size elementCount, void* buffer)
{
  if (!m_file)
  {
    throw FileNotOpenException();
  }
  return std::fread(buffer, elementSize, elementCount, m_file);
}

Size FileSystemFile::Write(Size elementSize, Size elementCount, const void* buffer)
{
  if (!m_file)
  {
    throw FileNotOpenException();
  }
  return std::fwrite(buffer, elementSize, elementCount, m_file);
}

void FileSystemFile::Close()
{
  if (m_file)
  {
    std::fclose(m_file);
    m_file = nullptr;
  }
}

FILE* FileSystemFile::GetHandle() const
{
  return m_file;
}

}