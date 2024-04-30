#ifndef COREMODULE_HH
#define COREMODULE_HH

#include <ceCore/coreexport.hh>

namespace ce
{
class CE_CORE_API CoreModule
{
public:
    static bool Register(int argc, char **argv);
    static bool Initialize(int argc, char **argv);
private:
  CoreModule() { }
};

}

extern "C" CE_CORE_API void initialize_core_module ();


#endif // COREMODULE_HH
