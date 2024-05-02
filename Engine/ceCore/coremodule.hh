#ifndef COREMODULE_HH
#define COREMODULE_HH

#include <ceCore/coreexport.hh>
#include <cecore/imodule.hh>

namespace ce
{
class CE_CORE_API CoreModule : public iModule
{
public:
  bool Register(int argc, char **argv, Engine *engine);
  bool Initialize(int argc, char **argv, Engine *engine);
};

}

CE_DECLARE_LIBRARY(ceCore);


#endif // COREMODULE_HH
