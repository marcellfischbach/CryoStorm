

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/resource/eAccessMode.hh>
#include <csCore/resource/eOpenMode.hh>
#include <csCore/resource/iFile.hh>
#include <csCore/csRef.hh>
#include <map>
#include <string>
#include <vector>

namespace cs
{


struct iArchive;
class csAssetLocator;
class CS_CORE_API csVFS
{
public:
  static csVFS *Get();
  void SetRootPath(const std::string &rootPath);
  CS_NODISCARD const std::string &GetRootPath() const;

  void AddArchive (iArchive* archive);
  void InsertAlias(const std::string &alias, const std::string &replacement);

  CS_NODISCARD const std::vector<csRef<const iArchive>> &GetArchives () const;

  CS_NODISCARD csOwned<iFile> Open(const csAssetLocator &resourceLocator, eAccessMode accessMode, eOpenMode openMode) const;

  bool IsMasterLocator (const csAssetLocator &resourceLocator) const;

private:
  csVFS();
  CS_NODISCARD iFile *File(const csAssetLocator &resourceLocator) const;
  CS_NODISCARD std::string ReplaceAliases(const std::string &str) const;

  std::vector<csRef<iArchive>>            m_archives;
  std::string                        m_rootPath;
  std::map<std::string, std::string> m_aliases;
};

}