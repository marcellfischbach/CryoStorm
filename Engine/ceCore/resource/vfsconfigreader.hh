

#pragma once

#include <ceCore/coreexport.hh>
#include <string>

namespace ce
{
class CrimsonFile;
class CrimsonFileElement;

class CE_CORE_API VFSConfigReader
{
private:
  VFSConfigReader();

public:
  static void Read(const std::string &configPath, const std::string &filename);

private:
  static void ReadConfig (const CrimsonFile *file);
  static void ReadVFS(const CrimsonFileElement *vfsElement);
  static void ReadArchives (const CrimsonFileElement *archivesElement);
  static void ReadArchive (const CrimsonFileElement *archiveElement);
  static void ReadAliases(const CrimsonFileElement *aliasesElement);
  static void ReadAlias(const CrimsonFileElement *aliasElement);
  static CrimsonFile *OpenConfigFile(const std::string &configPath, const std::string &filename) ;
};

} // ce
