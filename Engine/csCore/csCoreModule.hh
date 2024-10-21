#ifndef COREMODULE_HH
#define COREMODULE_HH

#include <csCore/csCoreExport.hh>
#include <csCore/iModule.hh>

namespace cs
{

CS_CLASS()
class CS_CORE_API csCoreModule : public CS_SUPER(iModule)
{
CS_CLASS_GEN_OBJECT;
public:
  static void RegisterClasses ();

  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);
};

}

CS_DECLARE_LIBRARY(ceCore);


#endif // COREMODULE_HH
