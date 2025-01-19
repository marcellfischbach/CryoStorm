
#include <csCore/animation/csSkeletonAnimationPlayer.hh>
#include <csCore/animation/csSkeletonAnimation.hh>
#include <csCore/graphics/csSkeleton.hh>

namespace cs
{

csSkeletonAnimationPlayer::csSkeletonAnimationPlayer()
    : iObject()
    , m_skeleton(nullptr)
    , m_animation(nullptr)
    , m_active(false)
    , m_time(0.0f)
{

}


csSkeletonAnimationPlayer::~csSkeletonAnimationPlayer()
{
}

void csSkeletonAnimationPlayer::SetSkeleton(cs::csSkeleton *skeleton)
{
  m_skeleton = skeleton;
}

void csSkeletonAnimationPlayer::SetAnimation(cs::csSkeletonAnimation *animation)
{
  m_animation = animation;
}


void csSkeletonAnimationPlayer::Start(float time)
{
  m_time   = time;
  m_active = true;
  UpdateFrame(m_time);
}

void csSkeletonAnimationPlayer::Update(float tpf)
{
  if (!m_active)
  {
    return;
  }

  m_time += tpf;
  float duration = m_animation->GetDuration();
  if (m_time > duration)
  {
    if (m_animation->IsLoop())
    {
      m_time -= duration;
    }
    else
    {
      m_time = duration;
    }
  }
  UpdateFrame(m_time);

}

void csSkeletonAnimationPlayer::UpdateFrame(float time)
{
  float    frame = time * m_animation->GetFramesPerSecond();
  for (int i     = 0; i < m_skeleton->GetNumberOfBones(); ++i)
  {
    csSkeleton::Bone &bone = m_skeleton->GetBone(i);
    bone.rotation = csQuaternion(0.0f, 0.0f, 0.0f, 0.0f);
  }

  m_animation->PushSkeleton(m_skeleton, frame, 1.0f);
  m_skeleton->UpdateBones();
}

}