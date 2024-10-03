
#include <ceCore/animation/skeletonanimationplayer.hh>
#include <ceCore/animation/skeletonanimation.hh>
#include <ceCore/graphics/skeleton.hh>

namespace ce
{

SkeletonAnimationPlayer::SkeletonAnimationPlayer()
    : iObject()
    , m_skeleton(nullptr)
    , m_animation(nullptr)
    , m_active(false)
    , m_time(0.0f)
{
  CS_CLASS_GEN_CONSTR;
}


SkeletonAnimationPlayer::~SkeletonAnimationPlayer()
{
  CS_RELEASE(m_skeleton);
  CS_RELEASE(m_animation);
  m_skeleton  = nullptr;
  m_animation = nullptr;
}

void SkeletonAnimationPlayer::SetSkeleton(ce::Skeleton *skeleton)
{
  CS_SET(m_skeleton, skeleton);
}

void SkeletonAnimationPlayer::SetAnimation(ce::SkeletonAnimation *animation)
{
  CS_SET(m_animation, animation);
}


void SkeletonAnimationPlayer::Start(float time)
{
  m_time   = time;
  m_active = true;
  UpdateFrame(m_time);
}

void SkeletonAnimationPlayer::Update(float tpf)
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

void SkeletonAnimationPlayer::UpdateFrame(float time)
{
  float    frame = time * m_animation->GetFramesPerSecond();
  for (int i     = 0; i < m_skeleton->GetNumberOfBones(); ++i)
  {
    Skeleton::Bone &bone = m_skeleton->GetBone(i);
    bone.rotation = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
  }

  m_animation->PushSkeleton(m_skeleton, frame, 1.0f);
  m_skeleton->UpdateBones();
}

}