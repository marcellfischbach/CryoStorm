//
// Created by Marcell on 01.07.2024.
//

#include <ceCore/loaders/textfileloader.hh>
#include <ceCore/resource/vfs.hh>
#include <ceCore/resource/textfile.hh>

namespace cryo
{


bool TextFileLoader::CanLoad(const cryo::Class *cls, const cryo::ResourceLocator &locator) const
{
  // text file loader can load everything
  return TextFile::GetStaticClass() == cls;
}

iObject *TextFileLoader::Load(const cryo::Class *cls, const cryo::ResourceLocator &locator) const
{
  iFile *file = VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
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
  auto textFile = new TextFile();
  textFile->SetContent(std::string(buffer));
  return textFile;
}

} // ce