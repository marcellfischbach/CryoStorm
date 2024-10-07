
#include <csAssimpLoader/csAssimpLoaderModule.hh>
#include <master.refl.cc>
#include <csAssimpLoader/csAssimpMeshLoader.hh>
#include <csAssimpLoader/csAssimpSkeletonAnimationLoader.hh>
#include <csAssimpLoader/csAssimpSkeletonMeshLoader.hh>

#include <csCore/csObjectRegistry.hh>
#include <csCore/resource/csAssetManager.hh>


namespace cs::assimp
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

CS_DEFINE_LIBRARY(csAssimpLoader)
{
  printf ("Load library: ceAssimpLoader\n");
  return new cs::assimp::csAssimpLoaderModule;
}