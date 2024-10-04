
#include <ceAssimpLoader/assimploadermodule.hh>
#include <master.refl.cc>
#include <ceAssimpLoader/assimpmeshloader.hh>
#include <ceAssimpLoader/assimpskeletonanimationloader.hh>
#include <ceAssimpLoader/assimpskeletonmeshloader.hh>

#include <ceCore/csObjectRegistry.hh>
#include <ceCore/resource/csAssetManager.hh>


namespace cryo::assimp
{

bool AssimpLoaderModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

  csAssetManager::Get()->RegisterLoader(new AssimpMeshLoader());
  csAssetManager::Get()->RegisterLoader(new AssimpSkeletonAnimationLoader());
  csAssetManager::Get()->RegisterLoader(new AssimpSkeletonMeshLoader());


  return true;
}

bool AssimpLoaderModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  return true;
}


}

CS_DEFINE_LIBRARY(ceAssimpLoader)
{
  printf ("Load library: ceAssimpLoader\n");
  return new cryo::assimp::AssimpLoaderModule;
}