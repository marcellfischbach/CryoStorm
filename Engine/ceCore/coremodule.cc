#include <ceCore/coremodule.hh>
#include <master.refl.cc>
#include <ceCore/objectregistry.hh>
#include <ceCore/graphics/defaultframerenderer.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/loaders/materialloader.hh>
#include <ceCore/loaders/samplerloader.hh>
#include <ceCore/loaders/terrainlayerloader.hh>
#include <ceCore/loaders/textureloader.hh>




namespace ce
{

bool CoreModule::Register(int argc, char** argv)
{
  register_classes();

  ObjectRegistry::Register<iFrameRenderer>(new DefaultFrameRenderer);
  AssetManager::Set(new AssetManager());
  AssetManager* assetMan = AssetManager::Get();
  assetMan->RegisterLoader(new SamplerLoader());
  assetMan->RegisterLoader(new MaterialLoader());
  assetMan->RegisterLoader(new TerrainLayerLoader());
  assetMan->RegisterLoader(new TerrainLayerMaskLoader());
  assetMan->RegisterLoader(new TextureLoader());

  return true;
}

bool CoreModule::Initialize(int argc, char** argv)
{

  return true;
}

}
