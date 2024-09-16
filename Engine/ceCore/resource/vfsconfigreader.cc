
#include <ceCore/resource/vfsconfigreader.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/resource/file.hh>
#include <ceCore/resource/filesystemarchive.hh>
#include <ceCore/resource/filesystemfile.hh>
#include <ceCore/resource/javaarchive.hh>
#include <ceCore/resource/resourcelocator.hh>
#include <ceCore/autoptr.hh>


#define CE_NOT_NULL(e) if (e == nullptr) return
namespace ce
{


VFSConfigReader::VFSConfigReader()
{
}

void VFSConfigReader::Read(const std::string &configPath, const std::string &filename)
{
  AutoDelete config(OpenConfigFile(configPath, filename));
  ReadConfig(config);
}

void VFSConfigReader::ReadConfig(const CrimsonFile* file)
{
  CE_NOT_NULL(file);

  const CrimsonFileElement* vfsElement = file->Root()->GetChild("vfs");
  ReadVFS(vfsElement);
}

void VFSConfigReader::ReadVFS(const CrimsonFileElement* vfsElement)
{
  CE_NOT_NULL(vfsElement);

  const CrimsonFileElement* archivesElement = vfsElement->GetChild("archives");
  ReadArchives(archivesElement);

  const CrimsonFileElement* aliasesElement = vfsElement->GetChild("aliases");
  ReadAliases(aliasesElement);


}

void VFSConfigReader::ReadArchives(const ce::CrimsonFileElement *archivesElement)
{
  CE_NOT_NULL(archivesElement);
  for (int i = 0, in=archivesElement->GetNumberOfChildren(); i<in; ++i)
  {
    const CrimsonFileElement *child = archivesElement->GetChild(i);
    ReadArchive(child);
  }
}

void VFSConfigReader::ReadArchive(const ce::CrimsonFileElement *archiveElement)
{
  if (archiveElement->GetTagName() == "filesystem" || archiveElement->GetTagName() == "fs")
  {
    std::string name = archiveElement->GetAttribute(0, "Root");
    int priority = archiveElement->GetAttribute(1, 0);
    std::string path = archiveElement->GetAttribute(2, "");
    if (!path.empty())
    {
      VFS::Get()->AddArchive (new FileSystemArchive(name, path, priority));
    }
  }
#ifdef CE_JAVA
  else if (archiveElement->GetTagName() == "java")
  {
    std::string name = archiveElement->GetAttribute(0, "Java");
    int priority = archiveElement->GetAttribute(1, 0);
    std::string path = archiveElement->GetAttribute(2, "");
    if (!path.empty())
    {
      VFS::Get()->AddArchive (new JavaArchive(name, path, priority));
    }
  }
#endif
}

void VFSConfigReader::ReadAliases(const CrimsonFileElement* aliasesElement)
{
  CE_NOT_NULL(aliasesElement);

  for (size_t i=0, in=aliasesElement->GetNumberOfChildren(); i<in; i++)
  {
    const CrimsonFileElement* child = aliasesElement->GetChild(i);
    ReadAlias(child);
  }
}

void VFSConfigReader::ReadAlias(const CrimsonFileElement* aliasElement)
{
  CE_NOT_NULL(aliasElement);

  const std::string& aliasName = aliasElement->GetTagName();
  const std::string &aliasReplacement = aliasElement->GetAttribute(0, "");
  if (!aliasReplacement.empty())
  {
    VFS::Get()->InsertAlias(aliasName, aliasReplacement);
  }
}

CrimsonFile* VFSConfigReader::OpenConfigFile(const std::string& configPath, const std::string &filename)
{
  std::string configFile = configPath + "/" + filename;
  AutoDelete<iFile> fsFile (new FileSystemFile(configFile));
  if (!fsFile)
  {
    printf ("VFS config file '%s' could not be found\n", configFile.c_str()); fflush(stdout);
    return nullptr;
  }
  printf ("Open VFS config file'%s'\n", configFile.c_str()); fflush(stdout);

  if  (!fsFile->Open(eAM_Read, eOM_Binary))
  {
    return nullptr;
  }

  CrimsonFile* file = new CrimsonFile();
  if (!file->Parse(fsFile))
  {
    delete file;
    return nullptr;
  }

  return file;
}

} // ce