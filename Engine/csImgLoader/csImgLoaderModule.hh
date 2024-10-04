
#pragma once

#include <csImgLoader/csImgLoaderExport.hh>
#include <csCore/iModule.hh>

namespace cryo::img
{

class CS_IMGLOADER_API csImgLoaderModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}

CS_DECLARE_LIBRARY(ceImgLoader);