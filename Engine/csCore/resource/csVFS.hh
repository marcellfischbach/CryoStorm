

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

  void SetRenderingApi (const std::string &renderingApi);
  const std::string &GetRenderingApi () const;

private:

  std::string ResolveAliases (const std::string &locator) const;

  csVFS();
  CS_NODISCARD iFile *File(const csAssetLocator &resourceLocator) const;

  std::vector<csRef<iArchive>>            m_archives;
  std::string                        m_rootPath;

  /// \brief Defines a custom path variable (gl4, dx11, vk, ...) that will replaced during opening time
  std::string m_renderingApi;
};

}