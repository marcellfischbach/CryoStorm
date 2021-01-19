
#include <spcAssimpLoader/assimploadermodule.hh>
#include <master.refl.cc>
#include <spcAssimpLoader/assimpmeshloader.hh>

#include <spcCore/objectregistry.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/graphics/image.hh>


namespace spc::assimp
{

bool AssimpLoaderModule::Register(int argc, char** argv)
{
  register_classes();

  AssetManager::Get()->RegisterLoader(new AssimpMeshLoader());


  return true;
}

bool AssimpLoaderModule::Initialize(int argc, char** argv)
{
  return true;
}


}