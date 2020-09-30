

#pragma once

#include <SpiceCore/coreexport.hh>
#include <string>

namespace Spice
{


enum eOpenMode
{
  eOM_Text,
  eOM_Binary,
};

enum eAccessMode
{
  eAM_Read,
  eAM_ReadWrite,
  eAM_Append,
};

class ResourceLocator;
class SPICE_CORE_API VFS
{
public:
  static VFS* Get();
  void SetBasePath(const std::string& basePath);
  const std::string& GetBasePath() const;

  FILE* Open(const ResourceLocator& resourceLocator, eAccessMode accessMode, eOpenMode openMode);

private:
  VFS();

  std::string m_basePath;
};


}