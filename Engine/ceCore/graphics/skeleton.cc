
#include <ceCore/graphics/skeleton.hh>

namespace ce
{

const std::string Skeleton::ILLEGAL_BONE_NAME;
const Matrix4f Skeleton::ILLEGAL_BONE_MATRIX = Matrix4f();

Skeleton::Skeleton()
: Object ()
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


  m_bones = skeleton.m_bones;
  m_skeletonBones = skeleton.m_skeletonBones;
  m_rootBones = skeleton.m_rootBones;
}

Skeleton& Skeleton::operator=(const ce::Skeleton &skeleton)
{
  Clear();

  m_bones = skeleton.m_bones;
  m_skeletonBones = skeleton.m_skeletonBones;
  m_rootBones = skeleton.m_rootBones;

  return *this;
}

size_t Skeleton::Add(const std::string &name)
{
  size_t idx = m_bones.size();

  Bone bone {
      idx,
      name,
      std::vector<size_t>(),
      Matrix4f()
  };
  m_bones.push_back(bone);
  m_skeletonBones.push_back(bone.matrix);
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

void Skeleton::SetBone(size_t idx, const ce::Matrix4f &localMatrix)
{
  if (idx < m_bones.size())
  {
    m_bones[idx].matrix = localMatrix;
  }
}

const Matrix4f &Skeleton::GetBone (size_t idx) const
{
  if (idx < m_bones.size())
  {
    return m_bones[idx].matrix;
  }
  return ILLEGAL_BONE_MATRIX;
}

const std::string &Skeleton::GetName (size_t idx) const
{
  if (idx < m_bones.size())
  {
    return m_bones[idx].name;
  }
  return ILLEGAL_BONE_NAME;
}

void Skeleton::UpdateBones()
{
  Matrix4f identity;
  for (const size_t &idx: m_rootBones)
  {
    UpdateBone(idx, identity);
  }
}

void Skeleton::UpdateBone(size_t idx, const ce::Matrix4f &parent)
{
  Bone &bone = m_bones[idx];

  Matrix4f global = parent * bone.matrix;
  m_skeletonBones[idx] = global;

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