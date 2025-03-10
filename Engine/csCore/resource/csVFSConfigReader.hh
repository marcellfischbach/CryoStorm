

#pragma once

#include <csCore/csCoreExport.hh>
#include <string>

namespace cs
{
namespace file
{
class csCryoFile;
class csCryoFileElement;
}

class CS_CORE_API csVFSConfigReader
{
private:
  csVFSConfigReader();

public:
  static void Read(const std::string &configPath, const std::string &filename);

private:
  static void ReadConfig(const file::csCryoFile* file);
  static void ReadVFS(const file::csCryoFileElement *vfsElement);
  static void ReadArchives(const file::csCryoFileElement *archivesElement);
  static void ReadArchive(const file::csCryoFileElement *archiveElement);
  static file::csCryoFile *OpenConfigFile(const std::string &configPath, const std::string &filename) ;
};

} // ce
