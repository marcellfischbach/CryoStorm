
#pragma once

#include <ceImgLoader/imgloaderexport.hh>
#include <ceCore/imodule.hh>

namespace cryo::img
{

class CS_IMGLOADER_API ImgLoaderModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, Engine *engine);
  bool Initialize(const std::vector<std::string> &args, Engine *engine);

};

}

CS_DECLARE_LIBRARY(ceImgLoader);