#ifndef COREMODULE_HH
#define COREMODULE_HH

#include <ceCore/csCoreExport.hh>
#include <cecore/iModule.hh>

namespace cryo
{
class CS_CORE_API csCoreModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);
};

}

CS_DECLARE_LIBRARY(ceCore);


#endif // COREMODULE_HH
