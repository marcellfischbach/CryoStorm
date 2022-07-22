//
// Created by MCEL on 22.07.2022.
//


#pragma once

namespace ce
{

namespace file
{
class File;
class Element;
}

class VFSConfigReader
{
private:
  VFSConfigReader();

public:
  static void Read ();

private:
  static void ReadConfig (const file::File *file);
  static void ReadVFS(const file::Element *vfsElement);
  static void ReadAliases(const file::Element *aliasesElement);
  static void ReadAlias(const file::Element *aliasElement);
  static file::File *OpenConfigFile() ;
};

} // ce
