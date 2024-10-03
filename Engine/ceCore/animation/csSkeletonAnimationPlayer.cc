
#include <ceCore/animation/csSkeletonAnimationPlayer.hh>
#include <ceCore/animation/csSkeletonAnimation.hh>
#include <ceCore/graphics/skeleton.hh>

namespace cryo
{

csSkeletonAnimationPlayer::csSkeletonAnimationPlayer()
    : iObject()
    , m_skeleton(nullptr)
    , m_animation(nullptr)
    , m_active(false)
    , m_time(0.0f)
{
  CS_CLASS_GEN_CONSTR;
}


csSkeletonAnimationPlayer::~csSkeletonAnimationPlayer()
{
  CS_RELEASE(m_skeleton);
  CS_RELEASE(m_animation);
  m_skeleton  = nullptr;
  m_animation = nullptr;
}

void csSkeletonAnimationPlayer::SetSkeleton(cryo::csSkeleton *skeleton)
{
  CS_SET(m_skeleton, skeleton);
}

void csSkeletonAnimationPlayer::SetAnimation(cryo::csSkeletonAnimation *animation)
{
  CS_SET(m_animation, animation);
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
    bone.rotation = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
  }

  m_animation->PushSkeleton(m_skeleton, frame, 1.0f);
  m_skeleton->UpdateBones();
}

}