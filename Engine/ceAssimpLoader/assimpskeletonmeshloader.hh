
#pragma once

#include <ceAssimpLoader/assimpexport.hh>
#include <ceCore/resource/iAssetLoader.hh>
#include <ceCore/math/csMatrix4f.hh>

struct aiMesh;
struct aiNode;

namespace cryo::assimp
{

struct SkeletonLoaderData;

CS_CLASS()
class CS_ASSIMP_API AssimpSkeletonMeshLoader : public CS_SUPER(iAssetLoader)
{
CS_CLASS_GEN_OBJECT;
public:
  AssimpSkeletonMeshLoader();
  virtual ~AssimpSkeletonMeshLoader() { }

  virtual bool CanLoad(const Class * cls, const csResourceLocator & locator) const;

  virtual iObject* Load(const Class * cls, const csResourceLocator & locator) const;

private:
  void ReadSkeleton(aiNode* node, const csMatrix4f &parentMatrix, SkeletonLoaderData& d) const;
  void ReadBone(aiNode* node, SkeletonLoaderData& d, size_t parentBoneID) const;
  void ReadMesh(aiNode* node, const csMatrix4f &parentMatrix, SkeletonLoaderData& d) const;


};


}