#include <csCore/csCoreModule.hh>
#include <master.refl.cc>
#include <csCore/csObjectRegistry.hh>
#include <csCore/graphics/csDefaultFrameRenderer.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/loaders/csMaterialLoader.hh>
#include <csCore/loaders/csSamplerLoader.hh>
#include <csCore/loaders/csShaderGraphLoader.hh>
#include <csCore/loaders/csTerrainLayerLoader.hh>
#include <csCore/loaders/csTextFileLoader.hh>
#include <csCore/loaders/csTextureLoader.hh>


void initialize_core_module()
{
  printf ("initialize_core_module\n");
}

namespace cs
{

bool csCoreModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();


  csObjectRegistry::Register<iFrameRenderer>(new csDefaultFrameRenderer);
  csAssetManager * assetMan = csAssetManager::Get();
  assetMan->RegisterLoader(new csMaterialLoader());
  assetMan->RegisterLoader(new csSamplerLoader());
  assetMan->RegisterLoader(new csShaderGraphLoader());
  assetMan->RegisterLoader(new csTerrainLayerLoader());
  assetMan->RegisterLoader(new csTerrainLayerMaskLoader());
  assetMan->RegisterLoader(new csTextFileLoader());
  assetMan->RegisterLoader(new csTextureLoader());

  return true;
}

bool csCoreModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{

  return true;
}



}


CS_DEFINE_LIBRARY(ceCore)
{
  printf ("Load library: ceCore\n");
  return new cs::csCoreModule;
}