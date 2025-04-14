
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/math/csMatrix4f.hh>
#include <csCore/math/csQuaternion.hh>
#include <csCore/resource/iAsset.hh>
#include <vector>


namespace cs
{


CS_CLASS()
class CS_CORE_API csSkeleton : public CS_SUPER(iAsset)
{
CS_CLASS_GEN_OBJECT;
public:
  struct Bone
  {
    size_t              id;
    std::string         name;
    std::vector<size_t> children;
    csVector3f          offset;
    csQuaternion        rotation;
    csQuaternion        poseRotation;
    csMatrix4f          globalMatrix;
  };

  static const size_t      ILLEGAL_BONE_ID = ~0x00;
  static const std::string ILLEGAL_BONE_NAME;
  static const csMatrix4f  ILLEGAL_BONE_MATRIX;

  csSkeleton();

  void Clear();
  void InitializeFrom();
  void InitializeFrom(const csSkeleton &skeleton);

  CS_NODISCARD size_t AddRoot(const std::string &name);
  CS_NODISCARD size_t AddChild(const std::string &name, size_t parent);

  void SetBase(const csMatrix4f &base);
  const csMatrix4f &GetBase() const;

  void UpdateBones();


  CS_NODISCARD size_t GetNumberOfBones() const;
  CS_NODISCARD size_t IndexOf(const std::string &name) const;

  CS_NODISCARD Bone &GetBone(size_t idx);
  CS_NODISCARD const Bone &GetBone(size_t idx) const;

  CS_NODISCARD const std::vector<csMatrix4f> &GetSkeletonBones() const;

  csSkeleton &operator=(const csSkeleton &skeleton);


private:
  void InitializePoseMatrices (size_t idx, const csMatrix4f &parent);
  size_t Add(const std::string &name);
  void UpdateBone(size_t idx, const csMatrix4f &parent);



private:
  std::vector<size_t>     m_rootBones;
  std::vector<Bone>       m_bones;
  std::vector<csMatrix4f> m_skeletonBones;
  std::vector<csMatrix4f> m_poseMatrices;
  csMatrix4f              m_base;
  static Bone             IllegalBone;
};

} // ce
