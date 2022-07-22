//
// Created by MCEL on 07.07.2022.
//

#include <ceCore/resource/basecefassetloader.hh>
#include <ceCore/resource/vfs.hh>

namespace ce
{

bool BaseCEFAssetLoader::Open(const ResourceLocator& locator, CrimsonFile &file)
{
  iFile* fd = ce::VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  AutoRelease arFD(fd);

  return file.Parse(fd);
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