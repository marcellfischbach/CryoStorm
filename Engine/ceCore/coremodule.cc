#include <ceCore/coremodule.hh>
#include <master.refl.cc>
#include <ceCore/objectregistry.hh>
#include <ceCore/graphics/defaultframerenderer.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/textfile.hh>
#include <ceCore/loaders/materialloader.hh>
#include <ceCore/loaders/samplerloader.hh>
#include <ceCore/loaders/terrainlayerloader.hh>
#include <ceCore/loaders/textfileloader.hh>
#include <ceCore/loaders/textureloader.hh>
#include <ceCore/resource/vfs.hh>


void initialize_core_module()
{
  printf ("initialize_core_module\n");
}

namespace ce
{

bool CoreModule::Register(const std::vector<std::string> &args, Engine *engine)
{
  register_classes();


  ObjectRegistry::Register<iFrameRenderer>(new DefaultFrameRenderer);
  AssetManager* assetMan = AssetManager::Get();
  assetMan->RegisterLoader(new SamplerLoader());
  assetMan->RegisterLoader(new MaterialLoader());
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


CE_DEFINE_LIBRARY(ceCore)
{
  printf ("Load library: ceCore\n");
  return new ce::CoreModule;
}