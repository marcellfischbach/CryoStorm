
#include <ceCore/graphics/skeletonmesh.hh>

namespace cryo
{

SkeletonMesh::SkeletonMesh()
: Mesh()
{

}




csSkeleton &SkeletonMesh::GetSkeleton()
{
  return m_skeleton;
}

const csSkeleton &SkeletonMesh::GetSkeleton() const
{
  return m_skeleton;
}


}
