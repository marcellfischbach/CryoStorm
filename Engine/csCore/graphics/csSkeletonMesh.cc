
#include <csCore/graphics/csSkeletonMesh.hh>

namespace cryo
{

csSkeletonMesh::csSkeletonMesh()
: csMesh()
{

}




csSkeleton &csSkeletonMesh::GetSkeleton()
{
  return m_skeleton;
}

const csSkeleton &csSkeletonMesh::GetSkeleton() const
{
  return m_skeleton;
}


}
