//
// Created by MCEL on 10.04.2025.
//

#include <csCore/entity/csSkeletonState.hh>
#include <csCore/graphics/csSkeleton.hh>

namespace cs
{

void csSkeletonState::SetSkeleton(cs::csSkeleton *skeleton)
{
  m_skeleton = skeleton;
}

csSkeleton *csSkeletonState::GetSkeleton()
{
  return m_skeleton;
}


const csSkeleton *csSkeletonState::GetSkeleton() const
{
  return m_skeleton;
}

}