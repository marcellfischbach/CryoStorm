
#pragma once

#include <ceImgLoader/imgloaderexport.hh>

namespace ce::img
{

class CE_IMGLOADER_API ImgLoaderModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  ImgLoaderModule() { }
};

}