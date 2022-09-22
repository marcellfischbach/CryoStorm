
#include <ceCore/resource/vfsconfigreader.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/resource/file.hh>
#include <ceCore/resource/resourcelocator.hh>
#include <ceCore/autoptr.hh>


#define CE_NOT_NULL(e) if (e == nullptr) return
namespace ce
{


VFSConfigReader::VFSConfigReader()
{
}

void VFSConfigReader::Read()
{
  AutoDelete config(OpenConfigFile());
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

  const CrimsonFileElement* aliasesElement = vfsElement->GetChild("aliases");
  ReadAliases(aliasesElement);
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

CrimsonFile* VFSConfigReader::OpenConfigFile()
{
  AutoDelete<iFile> fsFile = VFS::Get()->Open(ResourceLocator("/vfs.config"), eAM_Read, eOM_Binary);
  if (!fsFile)
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