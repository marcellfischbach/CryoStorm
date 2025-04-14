
#include <csCore/graphics/csSkeleton.hh>
#include <csCore/input/csInput.hh>

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
    : iAsset()
    , m_base()
{

}

void csSkeleton::Clear()
{
  m_bones.clear();
  m_skeletonBones.clear();
  m_rootBones.clear();
}

void csSkeleton::InitializeFrom()
{
  for (const size_t &idx: m_rootBones)
  {
    csMatrix4f id;
    InitializePoseMatrices(idx, id);
  }
//  m_poseMatrices  = m_skeletonBones;
//  for (auto &matrix: m_poseMatrices)
//  {
//    matrix.Invert();
//  }
  UpdateBones();
}

void csSkeleton::InitializePoseMatrices(size_t idx, const cs::csMatrix4f &parent)
{
  Bone &bone = m_bones[idx];

  csMatrix4f local;
  bone.rotation.ToMatrix4(local);
  local.SetTranslation(bone.offset);


  csMatrix4f global = parent * local;
  m_poseMatrices[idx] = global.Inverted();

  for (const size_t &childIdx: bone.children)
  {
    InitializePoseMatrices(childIdx, global);
  }
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

  for (const size_t &idx: m_rootBones)
  {
    csMatrix4f id;
    UpdateBone(idx, id);
  }
  for (auto &item: m_skeletonBones)
  {
    item = m_base * item;
  }
}

void csSkeleton::UpdateBone(size_t idx, const cs::csMatrix4f &parent)
{
  Bone &bone = m_bones[idx];
//  bone.rotation.Normalize();

  csMatrix4f local;
  bone.rotation.ToMatrix4(local);
  local.SetTranslation(bone.offset);


  csMatrix4f global = parent * local;
  bone.globalMatrix = global;
  m_skeletonBones[idx] = global * m_poseMatrices[idx];


  if (csInput::IsKeyPressed(eKey::eK_Space))
  {
//    printf ("%s: %.2f %.2f %.2f @ %.2f\n", bone.name.c_str(), bone.rotation.x, bone.rotation.y, bone.rotation.z, bone.rotation.w);
//    local.Debug("local");
//    global.Debug("Global");
//    m_skeletonBones[idx].Debug("Skeleton bone");
//    fflush(stdout);
  }

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