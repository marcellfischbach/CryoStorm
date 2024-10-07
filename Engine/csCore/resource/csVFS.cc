
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csVFSConfigReader.hh>
#include <csCore/resource/csResourceLocator.hh>
#include <csCore/resource/csCryoFile.hh>
#include <csCore/resource/csFileSystemFile.hh>
#include <csCore/resource/iArchive.hh>

#include <regex>

namespace cs
{


csVFS::csVFS()
{

}

csVFS* csVFS::Get()
{
  static csVFS vfs;
  return &vfs;
}

void csVFS::SetRootPath(const std::string &rootPath)
{
  m_rootPath = rootPath;
}

const std::string& csVFS::GetRootPath() const
{
  return m_rootPath;
}

void csVFS::AddArchive(cs::iArchive *archive)
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

const std::vector<const iArchive*> &csVFS::GetArchives() const
{
  return reinterpret_cast<const std::vector<const iArchive*> &>(m_archives);

}

void csVFS::InsertAlias(const std::string& alias, const std::string& replacement)
{
  m_aliases[alias] = replacement;
}

iFile* csVFS::Open(const csResourceLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode) const
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

iFile* csVFS::File(const csResourceLocator& resourceLocator) const
{
  std::string resourcePathWithReplacedAliases = ReplaceAliases(resourceLocator.Encoded());
  return new csFileSystemFile(m_rootPath + "/" + resourcePathWithReplacedAliases);
}

std::string csVFS::ReplaceAliases(const std::string& str) const
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