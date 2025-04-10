
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


  for (const auto &origBoneId: m_originBoneIndices)
  {
    csVector4i idx(boneMap[origBoneId.x],
                   boneMap[origBoneId.y],
                   boneMap[origBoneId.z],
                   boneMap[origBoneId.w]);
    newSkeletonBoneIds.push_back(idx);
  }

  m_mesh->Modify()->Update(eVertexStream::eVS_BoneIndices, newSkeletonBoneIds.data());

}

}
