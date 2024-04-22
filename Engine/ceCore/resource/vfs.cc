
#include <ceCore/resource/vfs.hh>
#include <ceCore/resource/vfsconfigreader.hh>
#include <ceCore/resource/resourcelocator.hh>
#include <ceCore/resource/file.hh>
#include <ceCore/resource/filesystemfile.hh>
#include <ceCore/resource/iarchive.hh>

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

void VFS::SetRootPath(const std::string& rootPath)
{
  m_rootPath = rootPath;

  VFSConfigReader::Read(rootPath);
}

const std::string& VFS::GetRootPath() const
{
  return m_rootPath;
}

void VFS::AddArchive(ce::iArchive *archive)
{
  if (archive)
  {
    archive->AddRef();
    m_archives.push_back(archive);

    struct
    {
      bool operator()(iArchive* a, iArchive *b) const { return a->GetPriority() > b->GetPriority(); }
    } customLess;
    std::sort(m_archives.begin(), m_archives.end(), customLess);
  }
}

void VFS::InsertAlias(const std::string& alias, const std::string& replacement)
{
  m_aliases[alias] = replacement;
}

iFile* VFS::Open(const ResourceLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode) const
{

  std::string resourcePathWithReplacedAliases = ReplaceAliases(resourceLocator.Encoded());
  for (const auto &archive: m_archives)
  {
    iFile* file = archive->Open(resourcePathWithReplacedAliases, accessMode, openMode);
    if (file)
    {
      return file;
    }
  }

  return nullptr;
}

iFile* VFS::File(const ResourceLocator& resourceLocator) const
{
  std::string resourcePathWithReplacedAliases = ReplaceAliases(resourceLocator.Encoded());
  return new FileSystemFile(m_rootPath + "/" + resourcePathWithReplacedAliases);
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