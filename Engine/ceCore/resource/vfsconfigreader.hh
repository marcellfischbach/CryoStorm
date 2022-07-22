//
// Created by MCEL on 22.07.2022.
//


#pragma once

namespace ce
{
class CrimsonFile;
class CrimsonFileElement;

class VFSConfigReader
{
private:
  VFSConfigReader();

public:
  static void Read ();

private:
  static void ReadConfig (const CrimsonFile *file);
  static void ReadVFS(const CrimsonFileElement *vfsElement);
  static void ReadAliases(const CrimsonFileElement *aliasesElement);
  static void ReadAlias(const CrimsonFileElement *aliasElement);
  static CrimsonFile *OpenConfigFile() ;
};

} // ce
