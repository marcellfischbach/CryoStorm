
#pragma once

#include <ceImgLoader/imgloaderexport.hh>
#include <ceCore/imodule.hh>

namespace ce::img
{

class CE_IMGLOADER_API ImgLoaderModule : public iModule
{
public:
  bool Register(int argc, char** argv, Engine *engine);
  bool Initialize(int argc, char** argv, Engine *engine);

};

}

CE_DECLARE_LIBRARY(ceImgLoader);