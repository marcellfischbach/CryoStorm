#include <spcCore/coremodule.hh>
#include <spcCore/master.refl.cc>

namespace spc
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
