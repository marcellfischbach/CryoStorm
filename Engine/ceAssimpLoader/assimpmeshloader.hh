
#pragma once

#include <ceAssimpLoader/assimpexport.hh>
#include <ceCore/resource/iAssetLoader.hh>
#include <ceCore/math/csMatrix4f.hh>

struct aiMesh;
struct aiNode;
struct aiMaterial;

namespace cryo
{
struct iMaterial;
}

namespace cryo::assimp
{

struct StaticLoaderData;

CS_CLASS()
class CS_ASSIMP_API AssimpMeshLoader : public CS_SUPER(iAssetLoader)
{
  CS_CLASS_GEN_OBJECT;
public:
  AssimpMeshLoader();
  virtual ~AssimpMeshLoader() { }

  virtual bool CanLoad(const csClass * cls, const csResourceLocator & locator) const;

  virtual iObject* Load(const csClass * cls, const csResourceLocator & locator) const;

private:
  void ReadNode(aiNode* node, const csMatrix4f &parentMatrix, StaticLoaderData& d) const;

};


}