////
//// Created by Marcell on 01.07.2024.
////
//
//#include <csCore/loaders/csTextFileLoader.hh>
//#include <csCore/resource/csVFS.hh>
//#include <csCore/resource/csTextFile.hh>
//
//namespace cs
//{
//
//
//bool csTextFileLoader::CanLoad(const cs::csClass *cls, const cs::csAssetLocator &locator) const
//{
//  // text file loader can load everything
//  return csTextFile::GetStaticClass() == cls;
//}
//
//iObject *csTextFileLoader::Load(const cs::csClass *cls, const cs::csAssetLocator &locator) const
//{
//  iFile *file = csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
//  if (!file)
//  {
//    return nullptr;
//  }
//
//  file->Seek(eSM_End, 0);
//  long size = file->Tell();
//  file->Seek(eSM_Set, 0);
//
//  char *buffer = new char[size + 1];
//  memset(buffer, 0, size + 1);
//
//  file->Read(sizeof(char), size, buffer);
//
//  CS_RELEASE(file);
//  auto textFile = new csTextFile();
//  textFile->SetContent(std::string(buffer));
//  return textFile;
//}
//
//} // ce