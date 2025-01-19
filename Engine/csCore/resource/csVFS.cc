
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csVFSConfigReader.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/resource/csCryoFile.hh>
#include <csCore/resource/csFileSystemFile.hh>
#include <csCore/resource/iArchive.hh>

#include <regex>
#include <iostream>
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
  printf ("csVFS::SetRootPath: '%s'\n", rootPath.c_str());
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
    m_archives.emplace_back(archive);

    struct
    {
      bool operator()(iArchive* a, iArchive *b) const { return a->GetPriority() > b->GetPriority(); }
    } customLess;
    std::sort(m_archives.begin(), m_archives.end(), customLess);
  }
}

const std::vector<csRef<const iArchive>> &csVFS::GetArchives() const
{
  return reinterpret_cast<const std::vector<csRef<const iArchive>> &>(m_archives);

}

void csVFS::InsertAlias(const std::string& alias, const std::string& replacement)
{
  m_aliases[alias] = replacement;
}

csOwned<iFile> csVFS::Open(const csAssetLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode) const
{

  const std::string &archiveName = resourceLocator.GetArchive();
  std::string resourcePathWithReplacedAliases = ReplaceAliases(resourceLocator.Encoded());
  for (const auto &archive: m_archives)
  {
    if (!archiveName.empty() && archive->GetName() != archiveName)
    {
      continue;
    }

    csOwned<iFile> file = archive->Open(resourcePathWithReplacedAliases, accessMode, openMode);
    if (file)
    {
      return file;
    }
  }
  std::cerr << "File " << resourceLocator.Encoded() << " could not be found in any archive." << std::endl;

  return nullptr;
}

iFile* csVFS::File(const csAssetLocator& resourceLocator) const
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

#ifdef CS_JAVA
#include <jni.h>

extern "C"
{

JNIEXPORT void
JNICALL Java_org_cryo_core_resource_VFS_nSetRootPath(JNIEnv *env, jclass cls, jstring rootPath)
{
  cs::csJava::Set(env);

  const char *ptr = env->GetStringUTFChars(rootPath, nullptr);
  cs::csVFS::Get()->SetRootPath(std::string(ptr));
  env->ReleaseStringUTFChars(rootPath, ptr);
}


JNIEXPORT jstring
JNICALL Java_org_cryo_core_resource_VFS_nGetRootPath(JNIEnv *env, jclass cls)
{
  cs::csJava::Set(env);
  const std::string &rootPath = cs::csVFS::Get()->GetRootPath();
  return env->NewStringUTF(rootPath.c_str());
}

}

#endif