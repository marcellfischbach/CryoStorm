
#include <csImporter/Importer.hh>

#include <iostream>
#include <vector>
#include <string>

// the importers
#include <csAssimpImporter/AssimpImporter.hh>

using namespace cs::imp;

void print_usage (const char* name, const std::vector<iImporter*> &importer)
{
  std::cout << name << " <import file> [options]" << std::endl;
  for (const auto &imp: importer)
  {
    imp->PrintUsage();
  }
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
