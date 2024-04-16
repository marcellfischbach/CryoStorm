
#pragma once

struct aiMaterial;

namespace ce
{
struct iMaterial;
}
namespace ce::assimp
{

class AssimpMaterialLoader
{

public:
  static iMaterial *Read(aiMaterial *material);
};

}
