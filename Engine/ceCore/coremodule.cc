#include <ceCore/coremodule.hh>
#include <master.refl.cc>
#include <ceCore/objectregistry.hh>
#include <ceCore/graphics/csDefaultFrameRenderer.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/loaders/materialloader.hh>
#include <ceCore/loaders/samplerloader.hh>
#include <ceCore/loaders/shadergraphloader.hh>
#include <ceCore/loaders/terrainlayerloader.hh>
#include <ceCore/loaders/textfileloader.hh>
#include <ceCore/loaders/textureloader.hh>


void initialize_core_module()
{
  printf ("initialize_core_module\n");
}

namespace cryo
{

bool CoreModule::Register(const std::vector<std::string> &args, Engine *engine)
{
  register_classes();


  ObjectRegistry::Register<iFrameRenderer>(new csDefaultFrameRenderer);
  AssetManager* assetMan = AssetManager::Get();
  assetMan->RegisterLoader(new MaterialLoader());
  assetMan->RegisterLoader(new SamplerLoader());
  assetMan->RegisterLoader(new ShaderGraphLoader());
  assetMan->RegisterLoader(new TerrainLayerLoader());
  assetMan->RegisterLoader(new TerrainLayerMaskLoader());
  assetMan->RegisterLoader(new TextFileLoader());
  assetMan->RegisterLoader(new TextureLoader());

  return true;
}

bool CoreModule::Initialize(const std::vector<std::string> &args, Engine *engine)
{

  return true;
}



}


CS_DEFINE_LIBRARY(ceCore)
{
  printf ("Load library: ceCore\n");
  return new cryo::CoreModule;
}