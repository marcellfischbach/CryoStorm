//
// Created by Marcell on 22.04.2024.
//

#include <ceCore/resource/filesystemarchive.hh>
#include <ceCore/resource/filesystemfile.hh>
#include <ceCore/resource/vfs.hh>

namespace ce
{

FileSystemArchive::FileSystemArchive(const std::string &rootPath, int priority)
    : m_rootPath(Resolve(rootPath))
    , m_priority(priority)
{

}


std::string FileSystemArchive::Resolve(const std::string &rootPath)
{
  std::string result = rootPath;
  size_t idx = result.find("${vfs::root}");
  if (idx != std::string::npos)
  {
    result = result.replace(idx, 12, VFS::Get()->GetRootPath());
  }
  return result + "/";
}

int FileSystemArchive::GetPriority() const
{
  return m_priority;
}

iFile* FileSystemArchive::Open(const std::string &locator, ce::eAccessMode accessMode, ce::eOpenMode openMode)
{
  FileSystemFile *file = new FileSystemFile(m_rootPath + locator);
  if (!file->Open(accessMode, openMode))
  {
    file->Release();
    file = nullptr;
  }
  return file;
}

} // ce