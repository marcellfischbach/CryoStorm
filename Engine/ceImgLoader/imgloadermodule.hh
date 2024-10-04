
#pragma once

#include <ceImgLoader/imgloaderexport.hh>
#include <ceCore/iModule.hh>

namespace cryo::img
{

class CS_IMGLOADER_API ImgLoaderModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}

CS_DECLARE_LIBRARY(ceImgLoader);