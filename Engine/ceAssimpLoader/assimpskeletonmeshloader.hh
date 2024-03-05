
#pragma once

#include <ceAssimpLoader/assimpexport.hh>
#include <ceCore/resource/iassetloader.hh>
#include <ceCore/math/matrix4f.hh>

struct aiMesh;
struct aiNode;

namespace ce::assimp
{

struct LoaderData;

CE_CLASS()
class CE_ASSIMP_API AssimpSkeletonMeshLoader : public CE_SUPER(iAssetLoader)
{
CE_CLASS_GEN_OBJECT;
public:
  AssimpSkeletonMeshLoader();
  virtual ~AssimpSkeletonMeshLoader() { }

  virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;

private:
  void ReadNode(aiNode* node, const Matrix4f &parentMatrix, LoaderData& d) const;


};


}