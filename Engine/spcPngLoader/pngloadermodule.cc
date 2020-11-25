
#include <spcPngLoader/pngloadermodule.hh>
#include <spcPngLoader/master.refl.cc>
#include <spcPngLoader/pngloader.hh>

#include <spcCore/objectregistry.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/graphics/image.hh>


namespace spc
{

bool PngLoaderModule::Register(int argc, char** argv)
{
  register_classes();

  AssetManager::Get()->RegisterLoader(new PngLoader());


  return true;
}

bool PngLoaderModule::Initialize(int argc, char** argv)
{
  return true;
}


}