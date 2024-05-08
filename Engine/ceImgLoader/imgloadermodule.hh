
#pragma once

#include <ceImgLoader/imgloaderexport.hh>
#include <ceCore/imodule.hh>

namespace ce::img
{

class CE_IMGLOADER_API ImgLoaderModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, Engine *engine);
  bool Initialize(const std::vector<std::string> &args, Engine *engine);

};

}

CE_DECLARE_LIBRARY(ceImgLoader);