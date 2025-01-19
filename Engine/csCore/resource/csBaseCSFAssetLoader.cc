//
// Created by MCEL on 07.07.2022.
//

#include <csCore/resource/csBaseCSFAssetLoader.hh>
#include <csCore/resource/csVFS.hh>

namespace cs
{



bool csBaseCSFAssetLoader::Open(const csAssetLocator& locator, csCryoFile &file)
{
  csRef<iFile> fd = cs::csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);

  return file.Parse(fd);
}



csOwned<iAsset> csBaseCSFAssetLoader::Load(const csAssetLocator &locator) const
{
  csCryoFile file;
  if (!Open(locator, file))
  {
    return nullptr;
  }

  return Load(&file, locator);
}

} // ce