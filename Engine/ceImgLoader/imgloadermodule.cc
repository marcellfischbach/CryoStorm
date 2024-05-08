
#include <ceImgLoader/imgloadermodule.hh>
#include <master.refl.cc>
#include <ceImgLoader/jpegloader.hh>
#include <ceImgLoader/pngloader.hh>

#include <ceCore/objectregistry.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/graphics/image.hh>

namespace ce::img
{

bool ImgLoaderModule::Register(const std::vector<std::string> &args, Engine *engine)
{
  register_classes();

  AssetManager::Get()->RegisterLoader(new JpegLoader());
  AssetManager::Get()->RegisterLoader(new PngLoader());


  return true;
}

bool ImgLoaderModule::Initialize(const std::vector<std::string> &args, Engine *engine)
{
  return true;
}


}

CE_DEFINE_LIBRARY(ceImgLoader)
{
  printf ("Load library: ceImgLoader\n");
  return new ce::img::ImgLoaderModule;
}