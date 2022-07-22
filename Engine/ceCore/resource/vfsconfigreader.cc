
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

void VFSConfigReader::ReadConfig(const file::File* file)
{
  CE_NOT_NULL(file);

  const file::Element* vfsElement = file->Root()->GetChild("vfs");
  ReadVFS(vfsElement);
}

void VFSConfigReader::ReadVFS(const file::Element* vfsElement)
{
  CE_NOT_NULL(vfsElement);

  const file::Element* aliasesElement = vfsElement->GetChild("aliases");
  ReadAliases(aliasesElement);
}

void VFSConfigReader::ReadAliases(const file::Element* aliasesElement)
{
  CE_NOT_NULL(aliasesElement);

  for (size_t i=0, in=aliasesElement->GetNumberOfChildren(); i<in; i++)
  {
    const file::Element* child = aliasesElement->GetChild(i);
    ReadAlias(child);
  }
}

void VFSConfigReader::ReadAlias(const file::Element* aliasElement)
{
  CE_NOT_NULL(aliasElement);

  const std::string& aliasName = aliasElement->GetTagName();
  const std::string &aliasReplacement = aliasElement->GetAttribute(0, "");
  if (!aliasReplacement.empty())
  {
    VFS::Get()->InsertAlias(aliasName, aliasReplacement);
  }
}

file::File* VFSConfigReader::OpenConfigFile()
{
  AutoDelete fsFile = VFS::Get()->Open(ResourceLocator("/vfs.config"), eAM_Read, eOM_Text);
  if (!fsFile)
  {
    return nullptr;
  }

  file::File* file = new file::File();
  if (!file->Parse(fsFile))
  {
    delete file;
    return nullptr;
  }

  return file;
}

} // ce