
#include <ceCore/graphics/skeleton.hh>

namespace ce
{


const std::string Skeleton::ILLEGAL_BONE_NAME;
const Matrix4f Skeleton::ILLEGAL_BONE_MATRIX = Matrix4f();

Skeleton::Bone Skeleton::IllegalBone = {
    ILLEGAL_BONE_ID,
    ILLEGAL_BONE_NAME,
    std::vector<size_t>(),
    Vector3f(),
    Quaternion()
};

Skeleton::Skeleton()
    : Object()
{
  CE_CLASS_GEN_CONSTR;
}

void Skeleton::Clear()
{
  m_bones.clear();
  m_skeletonBones.clear();
  m_rootBones.clear();
}

void Skeleton::InitializeFrom(const ce::Skeleton &skeleton)
{
  Clear();


  m_bones         = skeleton.m_bones;
  m_skeletonBones = skeleton.m_skeletonBones;
  m_rootBones     = skeleton.m_rootBones;
  m_poseMatrices  = skeleton.m_skeletonBones;
  for (auto &matrix: m_poseMatrices)
  {
    matrix.Invert();
  }
  UpdateBones();
}

Skeleton &Skeleton::operator=(const ce::Skeleton &skeleton)
{
  Clear();

  m_bones         = skeleton.m_bones;
  m_skeletonBones = skeleton.m_skeletonBones;
  m_rootBones     = skeleton.m_rootBones;
  m_poseMatrices  = skeleton.m_skeletonBones;
  for (auto &matrix: m_poseMatrices)
  {
    matrix.Invert();
  }
  UpdateBones();

  return *this;
}

size_t Skeleton::Add(const std::string &name)
{
  size_t idx = m_bones.size();

  Bone bone {
      idx,
      name,
      std::vector<size_t>(),
      Vector3f(),
      Quaternion()
  };
  m_bones.push_back(bone);
  m_skeletonBones.emplace_back();
  m_poseMatrices.emplace_back();
  return idx;
}


size_t Skeleton::AddRoot(const std::string &name)
{
  size_t idx = Add(name);
  m_rootBones.push_back(idx);
  return idx;
}

size_t Skeleton::AddChild(const std::string &name, size_t parent)
{
  size_t idx = Add(name);
  if (parent < m_bones.size())
  {
    Bone &parentBone = m_bones[parent];
    parentBone.children.push_back(idx);
  }
  return idx;
}

size_t Skeleton::GetNumberOfBones() const
{
  return m_bones.size();
}

size_t Skeleton::IndexOf(const std::string &name) const
{
  for (const auto &bone: m_bones)
  {
    if (name == bone.name)
    {
      return bone.id;
    }
  }
  return ILLEGAL_BONE_ID;
}

Skeleton::Bone &Skeleton::GetBone(size_t idx)
{
  if (idx >= m_bones.size())
  {
    return IllegalBone;
  }
  return m_bones[idx];
}


const Skeleton::Bone &Skeleton::GetBone(size_t idx) const
{
  if (idx >= m_bones.size())
  {
    return IllegalBone;
  }
  return m_bones[idx];
}

void Skeleton::UpdateBones()
{
  Matrix4f identity;
  identity.SetIdentity();
  for (const size_t &idx: m_rootBones)
  {
    UpdateBone(idx, identity);
  }
}

void Skeleton::UpdateBone(size_t idx, const ce::Matrix4f &parent)
{
  Bone &bone = m_bones[idx];

  Matrix4f local;
  bone.rotation.ToMatrix4(local);
  local.SetTranslation(bone.offset);


  Matrix4f global = parent * local;
  m_skeletonBones[idx] = global * m_poseMatrices[idx];

  for (const size_t &childIdx: bone.children)
  {
    UpdateBone(childIdx, global);
  }
}

const std::vector<Matrix4f> &Skeleton::GetSkeletonBones() const
{
  return m_skeletonBones;
}


} // ce