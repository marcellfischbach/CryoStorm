
#include <csCore/animation/csSkeletonAnimationPlayer.hh>
#include <csCore/animation/csSkeletonAnimationProperties.hh>
#include <csCore/animation/csSkeletonAnimationStrip.hh>
#include <csCore/graphics/csSkeleton.hh>
#include <csCore/input/csInput.hh>

namespace cs
{

csSkeletonAnimationPlayer::csSkeletonAnimationPlayer()
    : iObject()
    , m_skeleton(nullptr)
    , m_animation(nullptr)
    , m_active(false)
    , m_time(0.0f)
    , m_properties(new csSkeletonAnimationProperties())
{

}


csSkeletonAnimationPlayer::~csSkeletonAnimationPlayer()
{
  delete m_properties;
}

void csSkeletonAnimationPlayer::SetSkeleton(cs::csSkeleton *skeleton)
{
  m_skeleton = skeleton;
}

void csSkeletonAnimationPlayer::SetAnimation(cs::csSkeletonAnimationStrip *animation)
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
  if (!m_active || !m_animation)
  {
    return;
  }

  m_time += tpf / 1.0f;
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
  if (!m_animation || !m_skeleton)
  {
    return;
  }
  if (csInput::IsKeyPressed(eKey::eK_Space))
  {
    printf ("Update time: %f\n", time);
  }

  float    frame = time * m_animation->GetFramesPerSecond();
  for (int i     = 0; i < m_skeleton->GetNumberOfBones(); ++i)
  {
    csSkeleton::Bone &bone = m_skeleton->GetBone(i);
    bone.rotation = csQuaternion(0.0f, 0.0f, 0.0f, 0.0f);
    bone.position = csVector3f(0.0f, 0.0f, 0.0f);
  }

  m_animation->PushSkeleton(m_skeleton, frame, 1.0f);
  for (int i     = 0; i < m_skeleton->GetNumberOfBones(); ++i)
  {
    csSkeleton::Bone &bone = m_skeleton->GetBone(i);
    bone.rotation.Normalize();
  }


  m_skeleton->UpdateBones();
}

}