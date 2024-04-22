#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/resource/ifile.hh>

namespace ce
{

class ResourceLocator;

CE_CLASS()
struct CE_CORE_API iArchive : public CE_SUPER(iObject)
{
  CE_CLASS_GEN;
  ~iArchive() override = default;

  CE_NODISCARD virtual int GetPriority () const = 0;
  CE_NODISCARD virtual iFile* Open (const std::string &locator, eAccessMode accessMode, eOpenMode openMode) = 0;
};


}