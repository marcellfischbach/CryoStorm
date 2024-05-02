
#include <ceLauncher/launchermodule.hh>
#include <master.refl.cc>
#include <ceCore/objectregistry.hh>



bool LauncherModule::Register(int argc, char** argv, ce::Engine* engine)
{
  register_classes();


  return true;
}

bool LauncherModule::Initialize(int argc, char** argv, ce::Engine* engine)
{
  return true;
}


