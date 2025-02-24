#include <csCore/csCoreModule.hh>
#include <master.refl.cc>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/loaders/csMaterialLoader.hh>
#include <csCore/loaders/csRenderMeshLoader.hh>
#include <csCore/loaders/csSamplerLoader.hh>
#include <csCore/loaders/csShaderGraphLoader.hh>
#include <csCore/loaders/csTerrainLayerLoader.hh>
#include <csCore/loaders/csTextureLoader.hh>


void initialize_core_module()
{
  printf ("initialize_core_module\n");
}

namespace cs
{

void csCoreModule::RegisterClasses()
{
  register_classes();
}

bool csCoreModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  csAssetManager * assetMan = csAssetManager::Get();
  assetMan->RegisterLoader(new csMaterialLoader());
  assetMan->RegisterLoader(new csRenderMeshLoader());
  assetMan->RegisterLoader(new csSamplerLoader());
  assetMan->RegisterLoader(new csShaderGraphLoader());
  assetMan->RegisterLoader(new csTerrainLayerLoader());
  assetMan->RegisterLoader(new csTerrainLayerMaskLoader());
  assetMan->RegisterLoader(new csTextureLoader());

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