
#pragma once

#include <ceAssimpLoader/assimpexport.hh>
#include <ceCore/resource/iassetloader.hh>
#include <ceCore/math/matrix4f.hh>

struct aiMesh;
struct aiNode;

namespace ce::assimp
{

struct SkeletonLoaderData;

CS_CLASS()
class CS_ASSIMP_API AssimpSkeletonMeshLoader : public CS_SUPER(iAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  AssimpSkeletonMeshLoader();
  virtual ~AssimpSkeletonMeshLoader() { }

  virtual bool CanLoad(const Class * cls, const ResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const ResourceLocator & locator) const;

private:
  void ReadSkeleton(aiNode* node, const Matrix4f &parentMatrix, SkeletonLoaderData& d) const;
  void ReadBone(aiNode* node, SkeletonLoaderData& d, size_t parentBoneID) const;
  void ReadMesh(aiNode* node, const Matrix4f &parentMatrix, SkeletonLoaderData& d) const;


};


}