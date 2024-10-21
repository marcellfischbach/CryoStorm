
#pragma once

#include <csImgLoader/csImgLoaderExport.hh>
#include <csCore/iModule.hh>

namespace cs::img
{

CS_CLASS()
class CS_IMGLOADER_API csImgLoaderModule : public CS_SUPER(iModule)
{
CS_CLASS_GEN_OBJECT;
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}

CS_DECLARE_LIBRARY(csImgLoader);