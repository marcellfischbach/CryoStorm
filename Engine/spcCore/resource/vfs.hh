

#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/resource/eaccessmode.hh>
#include <spcCore/resource/eopenmode.hh>
#include <spcCore/resource/ifile.hh>
#include <string>

namespace spc
{



class ResourceLocator;
class SPC_CORE_API VFS
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