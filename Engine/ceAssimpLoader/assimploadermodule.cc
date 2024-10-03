
#include <ceAssimpLoader/assimploadermodule.hh>
#include <master.refl.cc>
#include <ceAssimpLoader/assimpmeshloader.hh>
#include <ceAssimpLoader/assimpskeletonanimationloader.hh>
#include <ceAssimpLoader/assimpskeletonmeshloader.hh>

#include <ceCore/objectregistry.hh>
#include <ceCore/resource/assetmanager.hh>


namespace ce::assimp
{

bool AssimpLoaderModule::Register(const std::vector<std::string> &args, Engine *engine)
{
  register_classes();

  AssetManager::Get()->RegisterLoader(new AssimpMeshLoader());
  AssetManager::Get()->RegisterLoader(new AssimpSkeletonAnimationLoader());
  AssetManager::Get()->RegisterLoader(new AssimpSkeletonMeshLoader());


  return true;
}

bool AssimpLoaderModule::Initialize(const std::vector<std::string> &args, Engine *engine)
{
  return true;
}


}

CS_DEFINE_LIBRARY(ceAssimpLoader)
{
  printf ("Load library: ceAssimpLoader\n");
  return new ce::assimp::AssimpLoaderModule;
}