
#include <ceAssimpLoader/csAssimpLoaderModule.hh>
#include <master.refl.cc>
#include <ceAssimpLoader/csAssimpMeshLoader.hh>
#include <ceAssimpLoader/csAssimpSkeletonAnimationLoader.hh>
#include <ceAssimpLoader/csAssimpSkeletonMeshLoader.hh>

#include <ceCore/csObjectRegistry.hh>
#include <ceCore/resource/csAssetManager.hh>


namespace cryo::assimp
{

bool csAssimpLoaderModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

  csAssetManager::Get()->RegisterLoader(new csAssimpMeshLoader());
  csAssetManager::Get()->RegisterLoader(new csAssimpSkeletonAnimationLoader());
  csAssetManager::Get()->RegisterLoader(new csAssimpSkeletonMeshLoader());


  return true;
}

bool csAssimpLoaderModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  return true;
}


}

CS_DEFINE_LIBRARY(ceAssimpLoader)
{
  printf ("Load library: ceAssimpLoader\n");
  return new cryo::assimp::csAssimpLoaderModule;
}