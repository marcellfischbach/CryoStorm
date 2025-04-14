//
// Created by MCEL on 10.04.2025.
//

#include <csCore/loaders/csSkeletonLoader.hh>
#include <csCore/graphics/csSkeleton.hh>

namespace cs
{

csSkeletonLoader::csSkeletonLoader()
{
  RegisterType("SKELETON");
}
csOwned<iAsset> csSkeletonLoader::Load(const file::csCryoFile *file, const cs::csAssetLocator &locator) const
{
  const file::csCryoFileElement *root = file->Root();
  if (!root)
  {
    return nullptr;
  }

  const file::csCryoFileElement *skeletonElement = root->GetChild("skeleton");
  if (!skeletonElement)
  {
    return nullptr;
  }

  csSkeleton  *skeleton = new csSkeleton();
  ReadTransform(skeletonElement->GetChild("transform"), skeleton, csSkeleton::ILLEGAL_BONE_ID);


  const file::csCryoFileElement *bonesElement = skeletonElement->GetChild("bones");
  for (size_t i         = 0, in = bonesElement->GetNumberOfChildren(); i < in; ++i)
  {
    const file::csCryoFileElement *boneElement = bonesElement->GetChild(i);
    if (boneElement->GetTagName() == "bone")
    {
      ScanBone(boneElement, skeleton, -1);
    }
  }

  skeleton->InitializeFrom();

  return skeleton;
}

void csSkeletonLoader::ScanBone(const file::csCryoFileElement *boneElement,
                                cs::csSkeleton *skeleton,
                                int32_t parentBondIdx) const
{
  if (!boneElement->HasAttribute("name"))
  {
    return;
  }

  std::string boneName = boneElement->GetAttribute("name", "");
  int32_t     boneIdx  = -1;
  if (parentBondIdx == -1)
  {
    boneIdx = (int32_t) skeleton->AddRoot(boneName);
  }
  else
  {
    boneIdx = (int32_t) skeleton->AddChild(boneName, parentBondIdx);
  }

  ReadTransform(boneElement->GetChild("transform"), skeleton, boneIdx);

  const file::csCryoFileElement *childrenElement = boneElement->GetChild("children");
  if (childrenElement)
  {
    for (size_t i = 0, in = childrenElement->GetNumberOfChildren(); i < in; ++i)
    {
      const file::csCryoFileElement *childBoneElement = childrenElement->GetChild(i);
      if (childBoneElement->GetTagName() == "bone")
      {
        ScanBone(childBoneElement, skeleton, boneIdx);
      }
    }
  }
}

void csSkeletonLoader::ReadTransform(const file::csCryoFileElement *transformElement,
                                     cs::csSkeleton *skeleton,
                                     size_t boneIdx) const
{
  if (!transformElement)
  {
    return;
  }

  csSkeleton::Bone &bone = skeleton->GetBone(boneIdx);


  const file::csCryoFileElement *matrixElement = transformElement->GetChild("matrix4");
  if (matrixElement)
  {
    float      m00 = matrixElement->GetAttribute(0, 1.0f);
    float      m01 = matrixElement->GetAttribute(1, 0.0f);
    float      m02 = matrixElement->GetAttribute(2, 0.0f);
    float      m03 = matrixElement->GetAttribute(3, 0.0f);
    float      m10 = matrixElement->GetAttribute(4, 0.0f);
    float      m11 = matrixElement->GetAttribute(5, 1.0f);
    float      m12 = matrixElement->GetAttribute(6, 0.0f);
    float      m13 = matrixElement->GetAttribute(7, 0.0f);
    float      m20 = matrixElement->GetAttribute(8, 0.0f);
    float      m21 = matrixElement->GetAttribute(9, 0.0f);
    float      m22 = matrixElement->GetAttribute(10, 1.0f);
    float      m23 = matrixElement->GetAttribute(11, 0.0f);
    float      m30 = matrixElement->GetAttribute(12, 0.0f);
    float      m31 = matrixElement->GetAttribute(13, 0.0f);
    float      m32 = matrixElement->GetAttribute(14, 0.0f);
    float      m33 = matrixElement->GetAttribute(15, 1.0f);
    csMatrix4f mat(m00, m01, m02, m03,
                   m10, m11, m12, m13,
                   m20, m21, m22, m23,
                   m30, m31, m32, m33);

    if (bone.id == csSkeleton::ILLEGAL_BONE_ID)
    {
      skeleton->SetBase(mat);
    }
    else
    {
      csVector3f translation;
      csMatrix3f rot;
      csVector3f scale;
      mat.ExtractTRS(translation, rot, scale);
      csQuaternion rotation = csQuaternion::FromMatrix(rot);
      bone.offset   = translation;
      bone.rotation = rotation;
      bone.poseRotation = rotation;
    }
  }
}

} // cs