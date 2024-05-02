
#pragma once

#include <ceAssimpLoader/assimpexport.hh>
#include <ceCore/imodule.hh>

namespace ce::assimp
{

class CE_ASSIMP_API AssimpLoaderModule : public iModule
{
public:
  bool Register(int argc, char** argv, Engine *engine);
  bool Initialize(int argc, char** argv, Engine *engine);

};

}

CE_DECLARE_LIBRARY(ceAssimpLoader);