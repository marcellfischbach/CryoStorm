
#pragma once

#include <ceAssimpLoader/assimpexport.hh>
#include <ceCore/imodule.hh>

namespace ce::assimp
{

class CE_ASSIMP_API AssimpLoaderModule : public iModule
{
public:
  bool Register(const std::vector<std::string> &args, Engine *engine);
  bool Initialize(const std::vector<std::string> &args, Engine *engine);

};

}

CE_DECLARE_LIBRARY(ceAssimpLoader);