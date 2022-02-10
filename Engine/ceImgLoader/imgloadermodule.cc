
#include <ceImgLoader/imgloadermodule.hh>
#include <master.refl.cc>
#include <ceImgLoader/jpegloader.hh>
#include <ceImgLoader/pngloader.hh>

#include <ceCore/objectregistry.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/image.hh>


namespace ce::img
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