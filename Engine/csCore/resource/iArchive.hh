#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
#include <csCore/resource/iFile.hh>

namespace cs
{

class csAssetLocator;

CS_CLASS()
struct CS_CORE_API iArchive : public CS_SUPER(iObject)
{
  CS_CLASS_GEN;
  ~iArchive() override = default;

  CS_NODISCARD virtual const std::string &GetName () const = 0;
  CS_NODISCARD virtual int GetPriority () const = 0;
  CS_NODISCARD virtual csOwned<iFile> Open (const std::string &locator, eAccessMode accessMode, eOpenMode openMode) = 0;
};


}