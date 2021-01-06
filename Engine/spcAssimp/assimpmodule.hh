
#pragma once

#include <spcAssimp/assimpexport.hh>

namespace spc::assimp
{

class SPC_ASSIMP_API AssimpModule
{
public:
  static bool Register(int argc, char** argv);
  static bool Initialize(int argc, char** argv);

private:
  AssimpModule() { }
};

}