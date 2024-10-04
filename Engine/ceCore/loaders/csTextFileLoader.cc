//
// Created by Marcell on 01.07.2024.
//

#include <ceCore/loaders/csTextFileLoader.hh>
#include <ceCore/resource/csVFS.hh>
#include <ceCore/resource/csTextFile.hh>

namespace cryo
{


bool csTextFileLoader::CanLoad(const cryo::csClass *cls, const cryo::csResourceLocator &locator) const
{
  // text file loader can load everything
  return csTextFile::GetStaticClass() == cls;
}

iObject *csTextFileLoader::Load(const cryo::csClass *cls, const cryo::csResourceLocator &locator) const
{
  iFile *file = csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (!file)
  {
    return nullptr;
  }

  file->Seek(eSM_End, 0);
  long size = file->Tell();
  file->Seek(eSM_Set, 0);

  char *buffer = new char[size + 1];
  memset(buffer, 0, size + 1);

  file->Read(sizeof(char), size, buffer);

  CS_RELEASE(file);
  auto textFile = new csTextFile();
  textFile->SetContent(std::string(buffer));
  return textFile;
}

} // ce