
#include <csCore/resource/csVFSConfigReader.hh>
#include <csCore/resource/csVFS.hh>
#include <csCore/resource/csFileSystemArchive.hh>
#include <csCore/resource/csFileSystemFile.hh>
#include <csCore/resource/csJavaArchive.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/csAutoDelete.hh>
#include <csCryoFile/csCryoFile.hh>


#define CS_NOT_NULL(e) if (e == nullptr) return
namespace cs
{


using namespace file;

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
      printf ("VFSConfigReader: '%s' %d -> '%s'\n", name.c_str(), priority, path.c_str());
      csVFS::Get()->AddArchive (new csJavaArchive(name, path, priority));
    }
  }
#endif
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
  if (!file->Parse(fsFile->ReadAll()))
  {
    delete file;
    return nullptr;
  }

  return file;
}

} // ce

#ifdef  CS_JAVA

#include <jni.h>

extern "C"
{

JNIEXPORT void
JNICALL Java_org_cryo_core_resource_CsVFSConfigReader_nRead(JNIEnv *env, jclass cls, jstring configPath, jstring fileName)
{
  const char *ptrConfigPath = env->GetStringUTFChars(configPath, nullptr);
  const char *ptrFileName = env->GetStringUTFChars(fileName, nullptr);

  cs::csVFSConfigReader::Read(std::string(ptrConfigPath), std::string(ptrFileName));

  env->ReleaseStringUTFChars(configPath, ptrConfigPath);
  env->ReleaseStringUTFChars(fileName, ptrFileName);

}


}

#endif