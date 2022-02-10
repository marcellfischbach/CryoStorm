
#include <ceAssimpLoader/assimploadermodule.hh>
#include <master.refl.cc>
#include <ceAssimpLoader/assimpmeshloader.hh>

#include <ceCore/objectregistry.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/image.hh>


namespace ce::assimp
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