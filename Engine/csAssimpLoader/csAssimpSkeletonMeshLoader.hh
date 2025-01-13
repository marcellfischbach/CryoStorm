//
//#pragma once
//
//#include <csAssimpLoader/csAssimpExport.hh>
//#include <csCore/resource/iAssetLoader.hh>
//#include <csCore/math/csMatrix4f.hh>
//
//struct aiMesh;
//struct aiNode;
//
//namespace cs::assimp
//{
//
//struct SkeletonLoaderData;
//
//CS_CLASS()
//class CS_ASSIMP_API csAssimpSkeletonMeshLoader : public CS_SUPER(iAssetLoader)
//{
//CS_CLASS_GEN_OBJECT;
//public:
//  csAssimpSkeletonMeshLoader();
//  virtual ~csAssimpSkeletonMeshLoader() { }
//
//  virtual bool CanLoad(const csClass * cls, const csAssetLocator & locator) const;
//
//  virtual iObject* Load(const csClass * cls, const csAssetLocator & locator) const;
//
//private:
//  void ReadSkeleton(aiNode* node, const csMatrix4f &parentMatrix, SkeletonLoaderData& d) const;
//  void ReadBone(aiNode* node, SkeletonLoaderData& d, size_t parentBoneID) const;
//  void ReadMesh(aiNode* node, const csMatrix4f &parentMatrix, SkeletonLoaderData& d) const;
//
//
//};
//
//
//}