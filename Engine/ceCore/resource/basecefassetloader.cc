//
// Created by MCEL on 07.07.2022.
//

#include <ceCore/resource/basecefassetloader.hh>
#include <ceCore/resource/vfs.hh>

namespace ce
{

void BaseCEFAssetLoader::AddValidFile(const Class* cls, const std::string& fileExtension)
{
  ValidFile validFile;
  validFile.Cls = cls;
  validFile.Extension = fileExtension;
  m_validFiles.emplace_back(validFile);
}

bool BaseCEFAssetLoader::Open(const ResourceLocator& locator, CrimsonFile &file)
{
  iFile* fd = ce::VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  AutoRelease arFD(fd);

  return file.Parse(fd);
}

bool BaseCEFAssetLoader::CanLoad(const Class* cls, const ResourceLocator& locator) const
{
  const std::string& extension = locator.GetExtension();
  for (auto &validFile : m_validFiles)
  {
    if (cls->IsAssignableFrom(validFile.Cls) &&  extension == validFile.Extension)
    {
      return true;
    }
  }
  return false;
}

iObject *BaseCEFAssetLoader::Load(const Class* cls, const ResourceLocator& locator) const
{
  CrimsonFile file;
  if (!Open(locator, file))
  {
    return nullptr;
  }

  return Load(&file, cls, locator);
}

} // ce