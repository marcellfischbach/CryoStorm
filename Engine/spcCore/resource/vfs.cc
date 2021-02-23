
#include <spcCore/resource/vfs.hh>
#include <spcCore/resource/resourcelocator.hh>
#include <spcCore/resource/filesystemfile.hh>

namespace spc
{


VFS::VFS()
{

}

VFS* VFS::Get()
{
  static VFS vfs;
  return &vfs;
}

void VFS::SetBasePath(const std::string& basePath)
{
  m_basePath = basePath;
}

const std::string& VFS::GetBasePath() const
{
  return m_basePath;
}



iFile* VFS::Open(const ResourceLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode)
{
  iFile* file = File(resourceLocator);
  if (!file)
  {
    return nullptr;
  }
  if (!file->Open(accessMode, openMode))
  {
    file->Release();
    return nullptr;
  }
  return file;
}


iFile* VFS::File(const ResourceLocator& resourceLocator)
{
  return new FileSystemFile(m_basePath + "/" + resourceLocator.Encoded());
}

}