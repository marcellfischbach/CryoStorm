
#pragma once

#include <ceAssimpLoader/assimpexport.hh>
#include <ceCore/resource/iassetloader.hh>
#include <ceCore/math/matrix4f.hh>

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

  virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;

private:
  void ReadNode(aiNode* node, const Matrix4f &parentMatrix, StaticLoaderData& d) const;

};


}