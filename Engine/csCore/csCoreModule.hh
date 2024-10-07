#ifndef COREMODULE_HH
#define COREMODULE_HH

#include <csCore/csCoreExport.hh>
#include <csCore/iModule.hh>

namespace cs
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
