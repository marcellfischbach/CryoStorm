
#pragma once

struct aiMaterial;

namespace cryo
{
struct iMaterial;
}
namespace cryo::assimp
{

class csAssimpMaterialLoader
{

public:
  static iMaterial *Read(aiMaterial *material);
};

}
