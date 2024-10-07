
#pragma once

struct aiMaterial;

namespace cs
{
struct iMaterial;
}
namespace cs::assimp
{

class csAssimpMaterialLoader
{

public:
  static iMaterial *Read(aiMaterial *material);
};

}
