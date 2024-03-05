
#include <ceCore/graphics/skeletonmesh.hh>

namespace ce
{

SkeletonMesh::SkeletonMesh()
: Mesh()
{

}




Skeleton &SkeletonMesh::GetSkeleton()
{
  return m_skeleton;
}

const Skeleton &SkeletonMesh::GetSkeleton() const
{
  return m_skeleton;
}


}
