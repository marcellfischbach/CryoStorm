#include <ceCore/coremodule.hh>
#include <master.refl.cc>
#include <ceCore/objectregistry.hh>
#include <ceCore/graphics/defaultframerenderer.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/loaders/materialloader.hh>
#include <ceCore/loaders/samplerloader.hh>
#include <ceCore/loaders/terrainlayerloader.hh>
#include <ceCore/loaders/textureloader.hh>
#include <ceCore/resource/vfs.hh>


void initialize_core_module()
{
  printf ("initialize_core_module\n");
}

namespace ce
{

bool CoreModule::Register(int argc, char** argv, Engine* engine)
{
  register_classes();


  ObjectRegistry::Register<iFrameRenderer>(new DefaultFrameRenderer);
  AssetManager* assetMan = AssetManager::Get();
  assetMan->RegisterLoader(new SamplerLoader());
  assetMan->RegisterLoader(new MaterialLoader());
  assetMan->RegisterLoader(new TerrainLayerLoader());
  assetMan->RegisterLoader(new TerrainLayerMaskLoader());
  assetMan->RegisterLoader(new TextureLoader());

  return true;
}

bool CoreModule::Initialize(int argc, char** argv, Engine *engine)
{

  return true;
}



}


CE_DEFINE_LIBRARY(ceCore)
{
  printf ("Load library: ceCore\n");
  return new ce::CoreModule;
}