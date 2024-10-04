
#pragma once

#include <csAssimpLoader/csAssimpExport.hh>
#include <csCore/iModule.hh>

namespace cryo::assimp
{

class CS_ASSIMP_API csAssimpLoaderModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, csEngine *engine);
  bool Initialize(const std::vector<std::string> &args, csEngine *engine);

};

}

CS_DECLARE_LIBRARY(ceAssimpLoader);