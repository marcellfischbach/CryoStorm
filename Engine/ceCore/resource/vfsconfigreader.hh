//
// Created by MCEL on 22.07.2022.
//


#pragma once

#include <string>

namespace ce
{
class CrimsonFile;
class CrimsonFileElement;

class VFSConfigReader
{
private:
  VFSConfigReader();

public:
  static void Read(const std::string &configPath);

private:
  static void ReadConfig (const CrimsonFile *file);
  static void ReadVFS(const CrimsonFileElement *vfsElement);
  static void ReadArchives (const CrimsonFileElement *archivesElement);
  static void ReadArchive (const CrimsonFileElement *archiveElement);
  static void ReadAliases(const CrimsonFileElement *aliasesElement);
  static void ReadAlias(const CrimsonFileElement *aliasElement);
  static CrimsonFile *OpenConfigFile(const std::string &configPath) ;
};

} // ce
