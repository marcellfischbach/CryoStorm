#include <spcCore/coremodule.hh>
//#include <spcCore/master.refl.cc>
#include <master.refl.cc>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/loaders/materialloader.hh>
#include <spcCore/loaders/samplerloader.hh>
#include <spcCore/loaders/textureloader.hh>


namespace spc
{

bool CoreModule::Register(int argc, char** argv)
{
  register_classes();

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
