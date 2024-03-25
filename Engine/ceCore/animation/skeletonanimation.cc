#include <ceCore/animation/skeletonanimation.hh>
#include <ceCore/graphics/skeleton.hh>

namespace ce
{

SkeletonAnimation::SkeletonAnimation()
    : m_name("")
    , m_numberOfFrames(0.0f)
    , m_framesPerSecond(24.0f)
    , m_loop(false)
    , m_channels()
{
  CE_CLASS_GEN_CONSTR;
}

SkeletonAnimation::~SkeletonAnimation()
{
  m_channels.clear();
}

void SkeletonAnimation::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &SkeletonAnimation::GetName() const
{
  return m_name;
}

void SkeletonAnimation::SetNumberOfFrames(float numberOfFrames)
{
  m_numberOfFrames = numberOfFrames;
}

float SkeletonAnimation::GetNumberOfFrames() const
{
  return m_numberOfFrames;
}

void SkeletonAnimation::SetFramesPerSecond(float framesPerSecond)
{
  m_framesPerSecond = framesPerSecond;
}

float SkeletonAnimation::GetFramesPerSecond() const
{
  return m_framesPerSecond;
}

void SkeletonAnimation::SetLoop(bool loop)
{
  m_loop = loop;
}

bool SkeletonAnimation::IsLoop() const
{
  return m_loop;
}

float SkeletonAnimation::GetDuration() const
{
  return m_numberOfFrames / m_framesPerSecond;
}

void skeleton_animation_update_bone_rotation(const SkeletonAnimation::Channel &channel,
                                             Skeleton::Bone &bone,
                                             float frame,
                                             float blendFactor)
{
  if (channel.rotations.empty())
  {
    return;
  }

  auto last = &channel.rotations[0];
  for (const auto &frameX1: channel.rotations)
  {
    if (frameX1.frame >= frame)
    {
      auto &frameX0 = *last;
      float a = (frame - frameX0.frame) / (frameX1.frame - frameX0.frame);

      Quaternion quat = Quaternion::Blend(frameX0.rotation, frameX1.rotation, a);
      bone.rotation = bone.rotation + (quat * bone.poseRotation) * blendFactor;
      return;
    }

    last = &frameX1;
  }

}

void skeleton_animation_update_bone_position(const SkeletonAnimation::Channel &channel,
                                             Skeleton::Bone &bone,
                                             float frame,
                                             float blendFactor)
{

}

void skeleton_animation_update_bone_scale(const SkeletonAnimation::Channel &channel,
                                          Skeleton::Bone &bone,
                                          float frame,
                                          float blendFactor)
{

}

void skeleton_animation_update_bone(const SkeletonAnimation::Channel &channel,
                                    Skeleton::Bone &bone,
                                    float frame,
                                    float blendFactor)
{
  skeleton_animation_update_bone_rotation(channel, bone, frame, blendFactor);
  skeleton_animation_update_bone_position(channel, bone, frame, blendFactor);
  skeleton_animation_update_bone_scale(channel, bone, frame, blendFactor);
}


void SkeletonAnimation::PushSkeleton(ce::Skeleton *skeleton, float frame, float blendFactor) const
{
  for (const auto &channel: m_channels)
  {
    Skeleton::Bone &bone = skeleton->GetBone(skeleton->IndexOf(channel.name));
    ce::skeleton_animation_update_bone(channel, bone, frame, blendFactor);
  }
}

void SkeletonAnimation::AddRotationFrame(const std::string &channelName,
                                         float frame,
                                         const Quaternion &rotation)
{
  FrameRotation rotFrame {
      frame,
      rotation,
  };

  Channel   &channel = GetChannel(channelName);
  for (auto it       = channel.rotations.begin(); it != channel.rotations.end(); it++)
  {
    if (it->frame > frame)
    {
      channel.rotations.insert(it, rotFrame);
      return;
    }
  }
  channel.rotations.push_back(rotFrame);
}

void SkeletonAnimation::AddPositionFrame(const std::string &channelName, float frame, const ce::Vector3f &position)
{
  FramePosition posFrame {
      frame,
      position,
  };

  Channel   &channel = GetChannel(channelName);
  for (auto it       = channel.positions.begin(); it != channel.positions.end(); it++)
  {
    if (it->frame > frame)
    {
      channel.positions.insert(it, posFrame);
      return;
    }
  }
  channel.positions.push_back(posFrame);
}


void SkeletonAnimation::AddScaleFrame(const std::string &channelName, float time, const ce::Vector3f &scale)
{
  FrameScale scaleFrame {
      time,
      scale,
  };

  Channel   &channel = GetChannel(channelName);
  for (auto it       = channel.scales.begin(); it != channel.scales.end(); it++)
  {
    if (it->frame > time)
    {
      channel.scales.insert(it, scaleFrame);
      return;
    }
  }
  channel.scales.push_back(scaleFrame);
}


SkeletonAnimation::Channel &SkeletonAnimation::GetChannel(const std::string &channelName)
{
  for (Channel &channel: m_channels)
  {
    if (channel.name == channelName)
    {
      return channel;
    }
  }

  Channel ch {
      channelName,
      std::vector<FrameRotation>(),
      std::vector<FramePosition>(),
      std::vector<FrameScale>(),
  };
  Channel &result = m_channels.emplace_back(ch);
  return result;
}


void SkeletonAnimationPack::AddAnimation(SkeletonAnimation *animation)
{
  if (animation)
  {
    CE_ADDREF(animation);
    m_animations.push_back(animation);
  }
}

SkeletonAnimation *SkeletonAnimationPack::Get(const std::string &name)
{
  for (auto &animation: m_animations)
  {
    if (animation->GetName() == name)
    {
      return animation;
    }
  }
  return nullptr;
}

}