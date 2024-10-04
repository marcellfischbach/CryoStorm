//
// Created by MCEL on 07.07.2022.
//

#include <ceCore/resource/csBaseCSFAssetLoader.hh>
#include <ceCore/resource/csVFS.hh>

namespace cryo
{

void csBaseCSFAssetLoader::AddValidFile(const Class* cls, const std::string& fileExtension)
{
  ValidFile validFile;
  validFile.Cls = cls;
  validFile.Extension = fileExtension;
  m_validFiles.emplace_back(validFile);
}

bool csBaseCSFAssetLoader::Open(const csResourceLocator& locator, csCryoFile &file)
{
  iFile* fd = cryo::csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  AutoRelease arFD(fd);

  return file.Parse(fd);
}

bool csBaseCSFAssetLoader::CanLoad(const Class* cls, const csResourceLocator& locator) const
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

iObject *csBaseCSFAssetLoader::Load(const Class* cls, const csResourceLocator& locator) const
{
  csCryoFile file;
  if (!Open(locator, file))
  {
    return nullptr;
  }

  return Load(&file, cls, locator);
}

} // ce