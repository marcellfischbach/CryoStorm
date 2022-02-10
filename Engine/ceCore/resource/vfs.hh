

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/resource/eaccessmode.hh>
#include <ceCore/resource/eopenmode.hh>
#include <ceCore/resource/ifile.hh>
#include <string>

namespace ce
{



class ResourceLocator;
class CE_CORE_API VFS
{
public:
  static VFS* Get();
  void SetBasePath(const std::string& basePath);
  const std::string& GetBasePath() const;

  iFile* Open(const ResourceLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode);
  iFile* File(const ResourceLocator& resourceLocator);

private:
  VFS();

  std::string m_basePath;
};


}