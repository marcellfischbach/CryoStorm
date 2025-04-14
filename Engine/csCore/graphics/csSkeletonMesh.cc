
#include <csCore/graphics/csSkeletonMesh.hh>

namespace cs
{

csSkeletonMesh::csSkeletonMesh()
    : csMesh()
    , m_skeleton(nullptr)
{

}

csMesh::SubMesh *csSkeletonMesh::CreateSubMesh()
{
  return new SubMesh();
}


void csSkeletonMesh::SetSkeleton(const cs::csSkeleton *skeleton)
{
  m_skeleton = skeleton;
  for (const auto subMesh: m_subMeshes)
  {
    static_cast<SubMesh *>(subMesh)->ApplySkeleton(skeleton);
  }
}


/* *********************************************************************p
 * *********************************************************************
 *
 *      csSkeletonMesh::SubMesh
 *
 * *********************************************************************
 * *********************************************************************/

csSkeletonMesh::SubMesh::SubMesh()
    : csMesh::SubMesh()
{

}

void csSkeletonMesh::SubMesh::SetOriginBoneIndices(const std::vector<csVector4i> &originBoneIndices)
{
  m_originBoneIndices = originBoneIndices;
}

const std::vector<csVector4i> &csSkeletonMesh::SubMesh::GetOriginBoneIndices() const
{
  return m_originBoneIndices;
}

void csSkeletonMesh::SubMesh::AddBone(cs::int32_t idx, const std::string &boneName)
{
  m_bones[idx] = boneName;
}

void csSkeletonMesh::SubMesh::ApplySkeleton(const cs::csSkeleton *skeleton)
{
  std::vector<csVector4i> newSkeletonBoneIds;
  newSkeletonBoneIds.reserve(m_originBoneIndices.size());
  std::map<int32_t, int32_t> boneMap;
  for (const auto            &e: m_bones)
  {
    boneMap[e.first] = static_cast<int32_t>(skeleton->IndexOf(e.second));
  }


  int32_t maxId = -1;
  int32_t minId = 100000;
  for (const auto &origBoneId: m_originBoneIndices)
  {
    csVector4i idx(boneMap[origBoneId.x],
                   boneMap[origBoneId.y],
                   boneMap[origBoneId.z],
                   boneMap[origBoneId.w]);
    newSkeletonBoneIds.push_back(idx);
    maxId = idx.x >= maxId ? idx.x : maxId;
    maxId = idx.y >= maxId ? idx.y : maxId;
    maxId = idx.z >= maxId ? idx.z : maxId;
    maxId = idx.w >= maxId ? idx.w : maxId;
    minId = idx.x <= minId ? idx.x : minId;
    minId = idx.y <= minId ? idx.y : minId;
    minId = idx.z <= minId ? idx.z : minId;
    minId = idx.w <= minId ? idx.w : minId;
  }

  m_mesh->Modify()->Update(eVertexStream::eVS_BoneIndices, newSkeletonBoneIds.data());

}

}
