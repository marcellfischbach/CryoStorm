

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/eaccessmode.hh>
#include <ceCore/resource/eopenmode.hh>
#include <ceCore/resource/ifile.hh>
#include <map>
#include <string>
#include <vector>

namespace ce
{


struct iArchive;
class ResourceLocator;
class CS_CORE_API VFS
{
public:
  static VFS *Get();
  void SetRootPath(const std::string &rootPath);
  CS_NODISCARD const std::string &GetRootPath() const;

  void AddArchive (iArchive* archive);
  void InsertAlias(const std::string &alias, const std::string &replacement);

  const std::vector<const iArchive*> &GetArchives () const;

  CS_NODISCARD iFile *Open(const ResourceLocator &resourceLocator, eAccessMode accessMode, eOpenMode openMode) const;

private:
  VFS();
  CS_NODISCARD iFile *File(const ResourceLocator &resourceLocator) const;
  CS_NODISCARD std::string ReplaceAliases(const std::string &str) const;

  std::vector<iArchive *>            m_archives;
  std::string                        m_rootPath;
  std::map<std::string, std::string> m_aliases;
};

}