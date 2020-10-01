#ifndef COREMODULE_HH
#define COREMODULE_HH

#include <spcCore/coreexport.hh>

namespace spc
{
class SPC_CORE_API CoreModule
{
public:
    static bool Register(int argc, char **argv);
    static bool Initialize(int argc, char **argv);
private:
  CoreModule() { }
};

}

#endif // COREMODULE_HH
