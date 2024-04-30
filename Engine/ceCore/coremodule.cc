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

bool CoreModule::Register(int argc, char** argv)
{
  register_classes();

  std::string basePath("../");
  for (int    i = 0; i < argc; i++)
  {
    std::string arg(argv[i]);
    if (arg == std::string("--data") && i + 1 < argc)
    {
      basePath = std::string(argv[++i]);
    }
  }
  printf("Starting with base-path: '%s'\n", basePath.c_str());
  ce::VFS::Get()->SetRootPath(basePath);

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
