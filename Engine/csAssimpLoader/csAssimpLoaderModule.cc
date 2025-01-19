
#include <csAssimpLoader/csAssimpLoaderModule.hh>
#include <csAssimpLoader/csAssimpMeshLoader.hh>
#include <csAssimpLoader/csAssimpSkeletonAnimationLoader.hh>
#include <csAssimpLoader/csAssimpSkeletonMeshLoader.hh>

#include <csCore/csObjectRegistry.hh>
#include <csCore/resource/csAssetManager.hh>

#include <master.refl.cc>

namespace cs::assimp
{

bool csAssimpLoaderModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

//  csAssetManager::Get()->RegisterLoader(new csAssimpMeshLoader());
//  csAssetManager::Get()->RegisterLoader(new csAssimpSkeletonAnimationLoader());
//  csAssetManager::Get()->RegisterLoader(new csAssimpSkeletonMeshLoader());


  return true;
}

bool csAssimpLoaderModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  return true;
}

void csAssimpLoaderModule::Shutdown(const std::vector<std::string> &args, cs::csEngine *engine)
{

}

}

CS_DEFINE_LIBRARY(csAssimpLoader)
{
  return new cs::assimp::csAssimpLoaderModule;
}