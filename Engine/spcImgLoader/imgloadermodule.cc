
#include <spcImgLoader/imgloadermodule.hh>
#include <master.refl.cc>
#include <spcImgLoader/jpegloader.hh>
#include <spcImgLoader/pngloader.hh>

#include <spcCore/objectregistry.hh>
#include <spcCore/resource/assetmanager.hh>
#include <spcCore/graphics/image.hh>


namespace spc::img
{

bool ImgLoaderModule::Register(int argc, char** argv)
{
  register_classes();

  AssetManager::Get()->RegisterLoader(new JpegLoader());
  AssetManager::Get()->RegisterLoader(new PngLoader());


  return true;
}

bool ImgLoaderModule::Initialize(int argc, char** argv)
{
  return true;
}


}