
#pragma once

struct aiMaterial;

namespace cryo
{
struct iMaterial;
}
namespace cryo::assimp
{

class AssimpMaterialLoader
{

public:
  static iMaterial *Read(aiMaterial *material);
};

}
