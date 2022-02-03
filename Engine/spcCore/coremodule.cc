#include <spcCore/coremodule.hh>
#include <master.refl.cc>
#include <spcCore/objectregistry.hh>
#include <spcCore/graphics/defaultframerenderer.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/loaders/materialloader.hh>
#include <spcCore/loaders/samplerloader.hh>
#include <spcCore/loaders/textureloader.hh>




namespace spc
{

bool CoreModule::Register(int argc, char** argv)
{
  register_classes();

  ObjectRegistry::Register<iFrameRenderer>(new DefaultFrameRenderer);
  AssetManager::Set(new AssetManager());
  AssetManager::Get()->RegisterLoader(new SamplerLoaderSpc());
  AssetManager::Get()->RegisterLoader(new MaterialLoaderSpc());
  AssetManager::Get()->RegisterLoader(new TextureLoaderSpc());

  return true;
}

bool CoreModule::Initialize(int argc, char** argv)
{

  return true;
}

}
