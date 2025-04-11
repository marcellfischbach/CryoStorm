#include <csCore/csCoreModule.hh>
#include <master.refl.cc>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/loaders/csMaterialLoader.hh>
#include <csCore/loaders/csMeshLoader.hh>
#include <csCore/loaders/csSamplerLoader.hh>
#include <csCore/loaders/csShaderGraphLoader.hh>
#include <csCore/loaders/csSkeletonLoader.hh>
#include <csCore/loaders/csSkeletonAnimationLoader.hh>
#include <csCore/loaders/csTerrainLayerLoader.hh>
#include <csCore/loaders/csTextureLoader.hh>
#include <csCore/loaders/scene/csSceneLoader.hh>
#include <csCore/loaders/scene/csStaticMeshStateLoader.hh>


namespace cs
{

void csCoreModule::RegisterClasses()
{
  register_classes();
}

bool csCoreModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  csAssetManager::Get()
      ->RegisterLoader(new csMaterialLoader())
      ->RegisterLoader(new csMeshLoader())
      ->RegisterLoader(new csSamplerLoader())
      ->RegisterLoader(new csSceneLoader())
      ->RegisterLoader(new csShaderGraphLoader())
      ->RegisterLoader(new csSkeletonLoader())
      ->RegisterLoader(new csSkeletonAnimationLoader())
      ->RegisterLoader(new csTerrainLayerLoader())
      ->RegisterLoader(new csTerrainLayerMaskLoader())
      ->RegisterLoader(new csTextureLoader());

  csEntityStateLoaderRegistry::Get()
      .Register(new csStaticMeshStateLoader());

  return true;
}

bool csCoreModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{

  return true;
}


void csCoreModule::Shutdown(const std::vector<std::string> &args, cs::csEngine *engine)
{
}


}


CS_DEFINE_LIBRARY(ceCore)
{
  return new cs::csCoreModule;
}