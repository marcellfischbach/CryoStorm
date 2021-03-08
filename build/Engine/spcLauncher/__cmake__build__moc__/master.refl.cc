

#include <stdarg.h>
#include "window\sdlinputsystem.hh"
#include "window\sdlinputsystem.refl.hh"


#include "window\sdlinputsystem.refl.cc"


#include <spcCore/classregistry.hh>

static void register_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // window\sdlinputsystem.hh
  reg->Register(spc::SDLInputSystemClass::Get());
}

static void unregister_classes()
{
  spc::ClassRegistry *reg = spc::ClassRegistry::Get();
  // window\sdlinputsystem.hh
  reg->Unregister(spc::SDLInputSystemClass::Get());
}

