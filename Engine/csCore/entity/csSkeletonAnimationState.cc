//
// Created by MCEL on 11.04.2025.
//

#include <csCore/entity/csSkeletonAnimationState.hh>
#include <csCore/entity/csSkeletonState.hh>
#include <csCore/animation/csSkeletonAnimationStrip.hh>
#include <csCore/animation/csSkeletonAnimationPlayer.hh>
#include <csCore/graphics/csSkeleton.hh>


namespace cs
{

csSkeletonAnimationState::csSkeletonAnimationState()
    : csEntityState()
    , m_player(new csSkeletonAnimationPlayer())
{
  SetNeedUpdate(true);
}

void csSkeletonAnimationState::SetSkeleton(cs::csSkeletonState *skeletonState)
{
  m_skeletonState = skeletonState;
  if (m_skeletonState)
  {
    m_player->SetSkeleton(skeletonState->GetSkeleton());
  }
  else
  {
    m_player->SetSkeleton(nullptr);
  }
}

void csSkeletonAnimationState::SetAnimation(cs::csSkeletonAnimationStrip *animation)
{
  m_animation = animation;
  if (m_animation)
  {
    m_player->SetAnimation(animation);
  }
  else
  {
    m_player->SetAnimation(nullptr);
  }
}

void csSkeletonAnimationState::SetActive(bool active)
{
  if (active)
  {
    m_active = true;
    m_player->Start(m_initialStartTime);
  }
  else
  {
    // not possible to stop atm
  }
}

bool  csSkeletonAnimationState::IsActive() const
{
  return m_active;
}

void csSkeletonAnimationState::SetInitialStartTime(float startTime)
{
  m_initialStartTime = startTime;
  if (m_active)
  {
    m_player->Start(m_initialStartTime);
  }
}

float csSkeletonAnimationState::GetInitialStartTime() const
{
  return m_initialStartTime;
}

void cs::csSkeletonAnimationState::Update(float tpf)
{
  m_player->Update(tpf);
}
} // cs