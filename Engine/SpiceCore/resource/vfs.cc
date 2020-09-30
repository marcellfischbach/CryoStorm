
#include <SpiceCore/resource/vfs.hh>
#include <SpiceCore/resource/resourcelocator.hh>

namespace Spice
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

FILE* VFS::Open(const ResourceLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode)
{
  std::string flag;

  switch (accessMode)
  {
  case eAM_Read:
    flag += "r";
    break;
  case eAM_ReadWrite:
    flag += "w";
    break;
  case eAM_Append:
    flag += "a";
    break;
  }

  switch (openMode)
  {
  case eOM_Binary:
    flag += "b";
    break;
  case eOM_Text:
    flag += "t";
  }

  return fopen((m_basePath + "/" + resourceLocator.GetLocator()).c_str(), flag.c_str());
}

}