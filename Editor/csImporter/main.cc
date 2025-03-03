
#include <csImporter/Importer.hh>
#include <csCryoFile/csCryoFile.hh>

#include <iostream>
#include <vector>
#include <string>

// the importers
#include <csAssimpImporter/AssimpImporter.hh>

#include <Windows.h>

using namespace cs::imp;
using namespace cs::file;

void print_usage (const char* name, const std::vector<iImporter*> &importer)
{
  std::cout << name << " <import file> [options]" << std::endl;
  for (const auto &imp: importer)
  {
    imp->PrintUsage();
  }
}

std::vector<uint8_t> randomData(uint32_t size)
{
  std::vector<uint8_t> d;
  d.reserve(size);
  for (uint32_t i = 0; i < size; i++)
  {
    d.push_back(i);
  }
  return d;
}

void test_cryo_file()
{
  csCryoFile cryoFile;

  csCryoFileElement* elemMesh = new csCryoFileElement();
  elemMesh->SetTagName("mesh");

  csCryoFileElement* elemMaterialSlots = new csCryoFileElement();
  elemMaterialSlots->SetTagName("materialSlots");

  csCryoFileElement* elemMaterialSlot = new csCryoFileElement();
  elemMaterialSlot->SetTagName("materialSlot");
  elemMaterialSlot->AddAttribute(csCryoFileAttribute("name", "Default", csCryoFileAttribute::AttributeType::String));
  elemMaterialSlot->AddAttribute(csCryoFileAttribute("locator", "/materials/Default.mat", csCryoFileAttribute::AttributeType::String));

  csCryoFileElement* elemSubMeshes= new csCryoFileElement();
  elemSubMeshes->SetTagName("subMeshes");



  csCryoFileElement* elemSubMesh = new csCryoFileElement();
  elemSubMesh->SetTagName("subMesh");
  elemSubMesh->AddAttribute(csCryoFileAttribute("Plain Text", csCryoFileAttribute::AttributeType::String));
  elemSubMesh->AddAttribute(csCryoFileAttribute("1.0", csCryoFileAttribute::AttributeType::Number));
  elemSubMesh->AddAttribute(csCryoFileAttribute("slot", "0", csCryoFileAttribute::AttributeType::Number));
  elemSubMesh->AddAttribute(csCryoFileAttribute("locator", "/materials/Default.mesh", csCryoFileAttribute::AttributeType::String));


  cryoFile.Root()->AddChild(elemMesh);
  elemMesh->AddChild(elemMaterialSlots);
  elemMaterialSlots->AddChild(elemMaterialSlot);
  elemMesh->AddChild(elemSubMeshes);
  elemSubMeshes->AddChild(elemSubMesh);


  cryoFile.AddData("Something", randomData(256));
  cryoFile.AddData("AnotherThing", randomData(256));

  std::ofstream out;
  out.open("F:\\DEV\\C++\\CryoStorm\\Data\\test.cryoFile", std::ios::out | std::ios::binary | std::ios::trunc);
  cryoFile.Write(out, true, 2);
  out.close();


  csCryoFile newFile;
  newFile.Parse("F:\\DEV\\C++\\CryoStorm\\Data\\test.cryoFile");

}


int main (int argc, char** argv)
{
  std::vector<iImporter*> importers;
  importers.push_back(new AssimpImporter);


  if (argc < 2)
  {
    print_usage(argv[0], importers);
    return -1;
  }

  const char* importFilename = argv[1];
  if (strlen(importFilename) >= 2 && importFilename[0] == '-' && importFilename[1] == '-')
  {
    print_usage(argv[0], importers);
    return -1;
  }

  std::fs::path filename (importFilename);


  std::vector<std::string> args;
  for (int i=2; i<argc; i++)
  {
    args.push_back(argv[i]);
  }





  for (const auto importer : importers)
  {
    if (importer->CanImport(filename, args))
    {
      if (!importer->Import(filename, args))
      {
        print_usage(argv[0], importers);
      }
      return 0;
    }
  }

  std::cout << "Found no importer for: " << filename << std::endl;
  print_usage(argv[0], importers);



  return 0;
}
