
#include <ceImgLoader/imgloadermodule.hh>
#include <master.refl.cc>
#include <ceImgLoader/jpegloader.hh>
#include <ceImgLoader/pngloader.hh>

#include <ceCore/csObjectRegistry.hh>
#include <ceCore/resource/csAssetManager.hh>
#include <ceCore/graphics/csImage.hh>

namespace cryo::img
{

bool ImgLoaderModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

  csAssetManager::Get()->RegisterLoader(new JpegLoader());
  csAssetManager::Get()->RegisterLoader(new PngLoader());


  return true;
}

bool ImgLoaderModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  return true;
}


}

CS_DEFINE_LIBRARY(ceImgLoader)
{
  printf ("Load library: ceImgLoader\n");
  return new cryo::img::ImgLoaderModule;
}