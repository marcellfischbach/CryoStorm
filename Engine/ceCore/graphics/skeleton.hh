
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/matrix4f.hh>
#include <vector>


namespace ce
{


CE_CLASS()
class CE_CORE_API Skeleton : public CE_SUPER(Object)
{
  CE_CLASS_GEN_OBJECT;
public:
  static const size_t ILLEGAL_ID = ~0x00;
  static const std::string ILLEGAL_NAME;
  static const Matrix4f ILLEGAL_MATRIX;

  Skeleton();

  void Clear();
  void InitializeFrom(const Skeleton& skeleton);

  CE_NODISCARD size_t AddRoot(const std::string &name);
  CE_NODISCARD size_t AddChild(const std::string &name, size_t parent);

  void UpdateBones ();



  size_t GetNumberOfBones () const;
  size_t IndexOf(const std::string & name) const;

  void SetBone (size_t idx, const Matrix4f& localMatrix);
  const Matrix4f &GetBone (size_t idx) const;

  const std::string &GetName (size_t idx) const;

  CE_NODISCARD const std::vector<Matrix4f> &GetSkeletonBones () const;

  Skeleton& operator=(const Skeleton& skeleton);

private:
  size_t Add(const std::string &name);
  void UpdateBone (size_t idx, const Matrix4f &parent);

  struct Bone
  {
    size_t              id;
    std::string         name;
    std::vector<size_t> children;
    Matrix4f            matrix;
  };

private:
  std::vector<size_t>   m_rootBones;
  std::vector<Bone>     m_bones;
  std::vector<Matrix4f> m_skeletonBones;
};

} // ce
