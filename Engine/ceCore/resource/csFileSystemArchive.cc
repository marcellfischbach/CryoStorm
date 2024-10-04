//
// Created by Marcell on 22.04.2024.
//

#include <ceCore/resource/csFileSystemArchive.hh>
#include <ceCore/resource/csFileSystemFile.hh>
#include <ceCore/resource/csVFS.hh>

namespace cryo
{

csFileSystemArchive::csFileSystemArchive(const std::string &name, const std::string &rootPath, int priority)
    : m_name(name)
    , m_rootPath(Resolve(rootPath))
    , m_priority(priority)
{

}

const std::string& csFileSystemArchive::GetName() const
{
  return m_name;
}

const const std::string &csFileSystemArchive::GetRootPath() const
{
  return m_rootPath;
}

std::string csFileSystemArchive::Resolve(const std::string &rootPath)
{
  std::string result = rootPath;
  size_t idx = result.find("${vfs::root}");
  if (idx != std::string::npos)
  {
    result = result.replace(idx, 12, csVFS::Get()->GetRootPath());
  }
  return result + "/";
}

int csFileSystemArchive::GetPriority() const
{
  return m_priority;
}

iFile* csFileSystemArchive::Open(const std::string &locator, cryo::eAccessMode accessMode, cryo::eOpenMode openMode)
{
  csFileSystemFile *file = new csFileSystemFile(m_rootPath + locator);
  if (!file->Open(accessMode, openMode))
  {
    file->Release();
    file = nullptr;
  }
  return file;
}

} // ce