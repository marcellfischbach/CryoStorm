
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csVFSConfigReader.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/resource/csFileSystemFile.hh>
#include <csCore/resource/iArchive.hh>
#include <csCryoFile/csCryoFile.hh>

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


csOwned<iFile> csVFS::Open(const csAssetLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode) const
{

  const std::string &archiveName = resourceLocator.GetArchive();
  std::string       resourcePath = resourceLocator.Canonical();
  resourcePath = ResolveAliases(resourcePath);
  for (const auto &archive: m_archives)
  {
    if (!archiveName.empty() && archive->GetName() != archiveName)
    {
      continue;
    }

    csOwned<iFile> file = archive->Open(resourcePath, accessMode, openMode);
    if (file)
    {
      return file;
    }
  }
  std::cerr << "File " << resourceLocator.Encoded() << " (" << resourcePath << ") could not be found in any archive." << std::endl;

  return nullptr;
}

iFile* csVFS::File(const csAssetLocator& resourceLocator) const
{
  std::string resource = resourceLocator.Encoded();
  return new csFileSystemFile(m_rootPath + "/" + resource);
}


bool csVFS::IsMasterLocator(const cs::csAssetLocator &resourceLocator) const
{
  std::string archiveName = resourceLocator.GetArchive();
  if (archiveName.empty())
  {
    // no specific archive set... so this is the master per definition
    return true;
  }
  std::string resource = resourceLocator.Canonical();

  for (const auto &archive : m_archives)
  {
    if (archive->IsExisting(resource))
    {
      if (archive->GetName() == archiveName)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  return true;
}


void csVFS::SetRenderingApi(const std::string &renderingApi)
{
  m_renderingApi = renderingApi;
}

const std::string &csVFS::GetRenderingApi () const
{
  return m_renderingApi;
}


static std::string replace (const std::string &line, const std::string &alias, const std::string &replacement)
{
  size_t idx = line.find(alias);
  if (idx == std::string::npos)
  {
    return line;
  }

  return line.substr(0, idx) + replacement + line.substr(idx + alias.length());

}


std::string csVFS::ResolveAliases(const std::string &locator) const
{
  std::string res = replace (locator, "${rendering-api}", m_renderingApi);

  return res;
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