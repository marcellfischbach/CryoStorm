#include <SpiceCore/coremodule.hh>
#include <SpiceCore/master.refl.cc>

namespace Spice
{

bool CoreModule::Register(int argc, char** argv)
{
  register_classes();
  return true;
}

bool CoreModule::Initialize(int argc, char** argv)
{
  return true;
}

}
