
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/matrix4f.hh>
#include <ceCore/math/quaternion.hh>
#include <vector>


namespace ce
{


CS_CLASS()
class CS_CORE_API Skeleton : public CS_SUPER(Object)
{
  CS_CLASS_GEN_OBJECT;
public:
  struct Bone
  {
    size_t              id;
    std::string         name;
    std::vector<size_t> children;
    Vector3f            offset;
    Quaternion          rotation;
    Quaternion          poseRotation;
    Matrix4f            globalMatrix;
  };

  static const size_t      ILLEGAL_BONE_ID = ~0x00;
  static const std::string ILLEGAL_BONE_NAME;
  static const Matrix4f    ILLEGAL_BONE_MATRIX;

  Skeleton();

  void Clear();
  void InitializeFrom(const Skeleton& skeleton);

  CS_NODISCARD size_t AddRoot(const std::string &name);
  CS_NODISCARD size_t AddChild(const std::string &name, size_t parent);

  void SetBase(const Matrix4f &base);
  const Matrix4f &GetBase () const;

  void UpdateBones ();



  CS_NODISCARD size_t GetNumberOfBones () const;
  CS_NODISCARD size_t IndexOf(const std::string & name) const;

  CS_NODISCARD Bone& GetBone(size_t idx);
  CS_NODISCARD const Bone& GetBone(size_t idx) const;

  CS_NODISCARD const std::vector<Matrix4f> &GetSkeletonBones () const;

  Skeleton& operator=(const Skeleton& skeleton);


private:
  size_t Add(const std::string &name);
  void UpdateBone (size_t idx, const Matrix4f &parent);



private:
  std::vector<size_t>   m_rootBones;
  std::vector<Bone>     m_bones;
  std::vector<Matrix4f> m_skeletonBones;
  std::vector<Matrix4f> m_poseMatrices;
  Matrix4f              m_base;
  static Bone           IllegalBone;
};

} // ce
