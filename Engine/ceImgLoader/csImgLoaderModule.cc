
#include <ceImgLoader/csImgLoaderModule.hh>
#include <master.refl.cc>
#include <ceImgLoader/csJpegLoader.hh>
#include <ceImgLoader/csPngLoader.hh>

#include <ceCore/csObjectRegistry.hh>
#include <ceCore/resource/csAssetManager.hh>
#include <ceCore/graphics/csImage.hh>

namespace cryo::img
{

bool csImgLoaderModule::Register(const std::vector<std::string> &args, csEngine *engine)
{
  register_classes();

  csAssetManager::Get()->RegisterLoader(new csJpegLoader());
  csAssetManager::Get()->RegisterLoader(new csPngLoader());


  return true;
}

bool csImgLoaderModule::Initialize(const std::vector<std::string> &args, csEngine *engine)
{
  return true;
}


}

CS_DEFINE_LIBRARY(ceImgLoader)
{
  printf ("Load library: ceImgLoader\n");
  return new cryo::img::csImgLoaderModule;
}