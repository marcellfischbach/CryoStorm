
#pragma once

#include <spcImgLoader/imgloaderexport.hh>

namespace spc
{

class SPC_IMGLOADER_API ImgLoaderModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  ImgLoaderModule() { }
};

}