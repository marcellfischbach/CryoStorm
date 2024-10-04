#include <ceCore/csCoreModule.hh>
#include <master.refl.cc>
#include <ceCore/csObjectRegistry.hh>
#include <ceCore/graphics/csDefaultFrameRenderer.hh>
#include <ceCore/resource/csAssetManager.hh>
#include <ceCore/loaders/csMaterialLoader.hh>
#include <ceCore/loaders/csSamplerLoader.hh>
#include <ceCore/loaders/csShaderGraphLoader.hh>
#include <ceCore/loaders/csTerrainLayerLoader.hh>
#include <ceCore/loaders/csTextFileLoader.hh>
#include <ceCore/loaders/csTextureLoader.hh>


void initialize_core_module()
{
  printf ("initialize_core_module\n");
}

namespace cryo
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
  return new cryo::csCoreModule;
}