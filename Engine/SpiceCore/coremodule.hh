#ifndef COREMODULE_HH
#define COREMODULE_HH

#include <SpiceCore/coreexport.hh>

namespace Spice
{
class SPICE_CORE_API CoreModule
{
public:
    static bool Register(int argc, char **argv);
    static bool Initialize(int argc, char **argv);
private:
  CoreModule() { }
};

}

#endif // COREMODULE_HH
