
#pragma once

#include <spcAssimpLoader/assimpexport.hh>

namespace spc::assimp
{

class SPC_ASSIMP_API AssimpLoaderModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  AssimpLoaderModule() { }
};

}