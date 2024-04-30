
#include <ceLauncher/launchermodule.hh>
#include <master.refl.cc>
#include <ceCore/objectregistry.hh>


namespace ce
{

bool LauncherModule::Register(int argc, char** argv)
{
  register_classes();


  return true;
}

bool LauncherModule::Initialize(int argc, char** argv)
{
  return true;
}

}