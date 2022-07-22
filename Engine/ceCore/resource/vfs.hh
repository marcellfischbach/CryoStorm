

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/eaccessmode.hh>
#include <ceCore/resource/eopenmode.hh>
#include <ceCore/resource/ifile.hh>
#include <map>
#include <string>

namespace ce
{

namespace file
{
class File;
}

class ResourceLocator;
class CE_CORE_API VFS
{
public:
  static VFS* Get();
  void SetBasePath(const std::string& basePath);
  CE_NODISCARD const std::string& GetBasePath() const;

  void InsertAlias(const std::string& alias, const std::string& replacement);

  CE_NODISCARD iFile* Open(const ResourceLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode) const;

private:
  VFS();
  CE_NODISCARD iFile* File(const ResourceLocator& resourceLocator) const;
  CE_NODISCARD std::string ReplaceAliases(const std::string& str) const;

  std::string                        m_basePath;
  std::map<std::string, std::string> m_aliases;
};

}