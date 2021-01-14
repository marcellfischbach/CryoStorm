#include <spcCore/coremodule.hh>
//#include <spcCore/master.refl.cc>
#include <master.refl.cc>
#include <spcCore/resource/assetmanager.hh>

namespace spc
{


bool CoreModule::Register(int argc, char** argv)
{
  register_classes();

  AssetManager::Set(new AssetManager());


  return true;
}

bool CoreModule::Initialize(int argc, char** argv)
{
  return true;
}

}
