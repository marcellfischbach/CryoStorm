

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/resource/eAccessMode.hh>
#include <ceCore/resource/eOpenMode.hh>
#include <ceCore/resource/iFile.hh>
#include <map>
#include <string>
#include <vector>

namespace cryo
{


struct iArchive;
class csResourceLocator;
class CS_CORE_API csVFS
{
public:
  static csVFS *Get();
  void SetRootPath(const std::string &rootPath);
  CS_NODISCARD const std::string &GetRootPath() const;

  void AddArchive (iArchive* archive);
  void InsertAlias(const std::string &alias, const std::string &replacement);

  const std::vector<const iArchive*> &GetArchives () const;

  CS_NODISCARD iFile *Open(const csResourceLocator &resourceLocator, eAccessMode accessMode, eOpenMode openMode) const;

private:
  csVFS();
  CS_NODISCARD iFile *File(const csResourceLocator &resourceLocator) const;
  CS_NODISCARD std::string ReplaceAliases(const std::string &str) const;

  std::vector<iArchive *>            m_archives;
  std::string                        m_rootPath;
  std::map<std::string, std::string> m_aliases;
};

}