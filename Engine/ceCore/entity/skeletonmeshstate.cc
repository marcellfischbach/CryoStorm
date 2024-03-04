//
// Created by MCEL on 04.03.2024.
//

#include <ceCore/entity/skeletonmeshstate.hh>
#include <ceCore/animation/skeleton.hh>


namespace ce
{


SkeletonMeshState::SkeletonMeshState()
: StaticMeshState()
, m_skeleton(nullptr)
{

}


SkeletonMeshState::~SkeletonMeshState ()
{
  CE_RELEASE(m_skeleton);
  m_skeleton = nullptr;
}


void SkeletonMeshState::SetSkeleton(ce::Skeleton *skeleton)
{
  CE_SET(m_skeleton, skeleton);
}

Skeleton* SkeletonMeshState::GetSkeleton()
{
  return m_skeleton;
}

const Skeleton* SkeletonMeshState::GetSkeleton() const
{
  return m_skeleton;
}

}
