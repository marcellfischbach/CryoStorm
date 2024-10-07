
#include <csCore/graphics/csSkeleton.hh>

namespace cs
{


const std::string csSkeleton::ILLEGAL_BONE_NAME;
const csMatrix4f csSkeleton::ILLEGAL_BONE_MATRIX = csMatrix4f();

csSkeleton::Bone csSkeleton::IllegalBone = {
    ILLEGAL_BONE_ID,
    ILLEGAL_BONE_NAME,
    std::vector<size_t>(),
    csVector3f(),
    csQuaternion(),
    csQuaternion(),
    csMatrix4f()
};

csSkeleton::csSkeleton()
    : csObject()
    , m_base()
{
  CS_CLASS_GEN_CONSTR;
}

void csSkeleton::Clear()
{
  m_bones.clear();
  m_skeletonBones.clear();
  m_rootBones.clear();
}

void csSkeleton::InitializeFrom(const cs::csSkeleton &skeleton)
{
  Clear();


  m_base          = skeleton.m_base;
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

csSkeleton &csSkeleton::operator=(const cs::csSkeleton &skeleton)
{
  Clear();

  m_base          = skeleton.m_base;
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

size_t csSkeleton::Add(const std::string &name)
{
  size_t idx = m_bones.size();

  Bone bone {
      idx,
      name,
      std::vector<size_t>(),
      csVector3f(),
      csQuaternion(),
      csQuaternion(),
      csMatrix4f()
  };
  m_bones.push_back(bone);
  m_skeletonBones.emplace_back();
  m_poseMatrices.emplace_back();
  return idx;
}


size_t csSkeleton::AddRoot(const std::string &name)
{
  size_t idx = Add(name);
  m_rootBones.push_back(idx);
  return idx;
}

size_t csSkeleton::AddChild(const std::string &name, size_t parent)
{
  size_t idx = Add(name);
  if (parent < m_bones.size())
  {
    Bone &parentBone = m_bones[parent];
    parentBone.children.push_back(idx);
  }
  return idx;
}

void csSkeleton::SetBase(const cs::csMatrix4f &base)
{
  m_base = base;
}

const csMatrix4f &csSkeleton::GetBase() const
{
  return m_base;
}

size_t csSkeleton::GetNumberOfBones() const
{
  return m_bones.size();
}

size_t csSkeleton::IndexOf(const std::string &name) const
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

csSkeleton::Bone &csSkeleton::GetBone(size_t idx)
{
  if (idx >= m_bones.size())
  {
    return IllegalBone;
  }
  return m_bones[idx];
}


const csSkeleton::Bone &csSkeleton::GetBone(size_t idx) const
{
  if (idx >= m_bones.size())
  {
    return IllegalBone;
  }
  return m_bones[idx];
}

void csSkeleton::UpdateBones()
{
  csMatrix4f identity;
  identity.SetIdentity();
  for (const size_t &idx: m_rootBones)
  {
    UpdateBone(idx, m_base);
  }
}

void csSkeleton::UpdateBone(size_t idx, const cs::csMatrix4f &parent)
{
  Bone &bone = m_bones[idx];

  csMatrix4f local;
  bone.rotation.ToMatrix4(local);
  local.SetTranslation(bone.offset);


  csMatrix4f global = parent * local;
  bone.globalMatrix = global;
  m_skeletonBones[idx] = global * m_poseMatrices[idx];

  for (const size_t &childIdx: bone.children)
  {
    UpdateBone(childIdx, global);
  }
}

const std::vector<csMatrix4f> &csSkeleton::GetSkeletonBones() const
{
  return m_skeletonBones;
}


} // ce