
#include <csCore/resource/csVFSConfigReader.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csCryoFile.hh>
#include <csCore/resource/csFileSystemArchive.hh>
#include <csCore/resource/csFileSystemFile.hh>
#include <csCore/resource/csJavaArchive.hh>
#include <csCore/resource/csResourceLocator.hh>
#include <csCore/csAutoDelete.hh>


#define CS_NOT_NULL(e) if (e == nullptr) return
namespace cs
{


csVFSConfigReader::csVFSConfigReader()
{
}

void csVFSConfigReader::Read(const std::string &configPath, const std::string &filename)
{
  csAutoDelete config(OpenConfigFile(configPath, filename));
  ReadConfig(config);
}

void csVFSConfigReader::ReadConfig(const csCryoFile* file)
{
  CS_NOT_NULL(file);

  const csCryoFileElement * vfsElement = file->Root()->GetChild("vfs");
  ReadVFS(vfsElement);
}

void csVFSConfigReader::ReadVFS(const csCryoFileElement* vfsElement)
{
  CS_NOT_NULL(vfsElement);

  const csCryoFileElement * archivesElement = vfsElement->GetChild("archives");
  ReadArchives(archivesElement);

  const csCryoFileElement * aliasesElement = vfsElement->GetChild("aliases");
  ReadAliases(aliasesElement);


}

void csVFSConfigReader::ReadArchives(const cs::csCryoFileElement *archivesElement)
{
  CS_NOT_NULL(archivesElement);
  for (int i = 0, in=archivesElement->GetNumberOfChildren(); i<in; ++i)
  {
    const csCryoFileElement *child = archivesElement->GetChild(i);
    ReadArchive(child);
  }
}

void csVFSConfigReader::ReadArchive(const cs::csCryoFileElement *archiveElement)
{
  if (archiveElement->GetTagName() == "filesystem" || archiveElement->GetTagName() == "fs")
  {
    std::string name = archiveElement->GetAttribute(0, "Root");
    int priority = archiveElement->GetAttribute(1, 0);
    std::string path = archiveElement->GetAttribute(2, "");
    if (!path.empty())
    {
      csVFS::Get()->AddArchive (new csFileSystemArchive(name, path, priority));
    }
  }
#ifdef CS_JAVA
  else if (archiveElement->GetTagName() == "java")
  {
    std::string name = archiveElement->GetAttribute(0, "Java");
    int priority = archiveElement->GetAttribute(1, 0);
    std::string path = archiveElement->GetAttribute(2, "");
    if (!path.empty())
    {
      csVFS::Get()->AddArchive (new csJavaArchive(name, path, priority));
    }
  }
#endif
}

void csVFSConfigReader::ReadAliases(const csCryoFileElement* aliasesElement)
{
  CS_NOT_NULL(aliasesElement);

  for (size_t i=0, in=aliasesElement->GetNumberOfChildren(); i<in; i++)
  {
    const csCryoFileElement * child = aliasesElement->GetChild(i);
    ReadAlias(child);
  }
}

void csVFSConfigReader::ReadAlias(const csCryoFileElement* aliasElement)
{
  CS_NOT_NULL(aliasElement);

  const std::string& aliasName = aliasElement->GetTagName();
  const std::string &aliasReplacement = aliasElement->GetAttribute(0, "");
  if (!aliasReplacement.empty())
  {
    csVFS::Get()->InsertAlias(aliasName, aliasReplacement);
  }
}

csCryoFile* csVFSConfigReader::OpenConfigFile(const std::string& configPath, const std::string &filename)
{
  std::string         configFile = configPath + "/" + filename;
  csAutoDelete<iFile> fsFile (new csFileSystemFile(configFile));
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

  csCryoFile * file = new csCryoFile();
  if (!file->Parse(fsFile))
  {
    delete file;
    return nullptr;
  }

  return file;
}

} // ce