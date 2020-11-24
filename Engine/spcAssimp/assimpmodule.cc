
#include <spcAssimp/assimpmodule.hh>
#include <spcAssimp/master.refl.cc>
#include <spcAssimp/assimpimageloader.hh>

#include <spcCore/objectregistry.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/graphics/image.hh>


namespace spc
{

bool AssimpModule::Register(int argc, char** argv)
{
  register_classes();

  AssetManager::Get()->RegisterLoader<Image>(new AssimpImageLoader());


  return true;
}

bool AssimpModule::Initialize(int argc, char** argv)
{
  return true;
}


}