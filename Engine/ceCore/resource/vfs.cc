
#include <ceCore/resource/vfs.hh>
#include <ceCore/resource/vfsconfigreader.hh>
#include <ceCore/resource/resourcelocator.hh>
#include <ceCore/resource/file.hh>
#include <ceCore/resource/filesystemfile.hh>
#include <regex>

namespace ce
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

  VFSConfigReader::Read();
}

const std::string& VFS::GetBasePath() const
{
  return m_basePath;
}

void VFS::InsertAlias(const std::string& alias, const std::string& replacement)
{
  m_aliases[alias] = replacement;
}

iFile* VFS::Open(const ResourceLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode) const
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

iFile* VFS::File(const ResourceLocator& resourceLocator) const
{
  std::string resourcePathWithReplacedAliases = ReplaceAliases(resourceLocator.Encoded());
  return new FileSystemFile(m_basePath + "/" + resourcePathWithReplacedAliases);
}

std::string VFS::ReplaceAliases(const std::string& str) const
{
  std::string result(str);

  std::regex  reg("(.*)\\$\\{(.*)\\}(.*)");
  std::smatch sm;
  while (std::regex_match(result, sm, reg))
  {
    std::string front     = sm[1];
    std::string aliasName = sm[2];
    std::string back      = sm[3];

    std::string aliasReplacement = "";
    auto        aliasIt          = m_aliases.find(aliasName);
    if (aliasIt != m_aliases.end())
    {
      aliasReplacement = aliasIt->second;
    }

    result = std::string()
      .append(front)
      .append(aliasReplacement)
      .append(back);
  }

  return result;
}

}