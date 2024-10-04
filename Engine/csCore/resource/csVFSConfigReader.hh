

#pragma once

#include <csCore/csCoreExport.hh>
#include <string>

namespace cryo
{
class csCryoFile;
class csCryoFileElement;

class CS_CORE_API csVFSConfigReader
{
private:
  csVFSConfigReader();

public:
  static void Read(const std::string &configPath, const std::string &filename);

private:
  static void ReadConfig(const csCryoFile *file);
  static void ReadVFS(const csCryoFileElement *vfsElement);
  static void ReadArchives(const csCryoFileElement *archivesElement);
  static void ReadArchive(const csCryoFileElement *archiveElement);
  static void ReadAliases(const csCryoFileElement *aliasesElement);
  static void ReadAlias(const csCryoFileElement *aliasElement);
  static csCryoFile *OpenConfigFile(const std::string &configPath, const std::string &filename) ;
};

} // ce
