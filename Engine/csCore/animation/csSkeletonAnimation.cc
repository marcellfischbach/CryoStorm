#include <csCore/animation/csSkeletonAnimation.hh>
#include <csCore/graphics/csSkeleton.hh>

namespace cryo
{

csSkeletonAnimation::csSkeletonAnimation()
    : m_name("")
    , m_numberOfFrames(0.0f)
    , m_framesPerSecond(24.0f)
    , m_loop(false)
    , m_channels()
{
  CS_CLASS_GEN_CONSTR;
}

csSkeletonAnimation::~csSkeletonAnimation()
{
  m_channels.clear();
}

void csSkeletonAnimation::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &csSkeletonAnimation::GetName() const
{
  return m_name;
}

void csSkeletonAnimation::SetNumberOfFrames(float numberOfFrames)
{
  m_numberOfFrames = numberOfFrames;
}

float csSkeletonAnimation::GetNumberOfFrames() const
{
  return m_numberOfFrames;
}

void csSkeletonAnimation::SetFramesPerSecond(float framesPerSecond)
{
  m_framesPerSecond = framesPerSecond;
}

float csSkeletonAnimation::GetFramesPerSecond() const
{
  return m_framesPerSecond;
}

void csSkeletonAnimation::SetLoop(bool loop)
{
  m_loop = loop;
}

bool csSkeletonAnimation::IsLoop() const
{
  return m_loop;
}

float csSkeletonAnimation::GetDuration() const
{
  return m_numberOfFrames / m_framesPerSecond;
}

void skeleton_animation_update_bone_rotation(const csSkeletonAnimation::Channel &channel,
                                             csSkeleton::Bone &bone,
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

      csQuaternion quat = csQuaternion::Blend(frameX0.rotation, frameX1.rotation, a);
      bone.rotation = bone.rotation + quat  * blendFactor;
      return;
    }

    last = &frameX1;
  }

}

void skeleton_animation_update_bone_position(const csSkeletonAnimation::Channel &channel,
                                             csSkeleton::Bone &bone,
                                             float frame,
                                             float blendFactor)
{

}

void skeleton_animation_update_bone_scale(const csSkeletonAnimation::Channel &channel,
                                          csSkeleton::Bone &bone,
                                          float frame,
                                          float blendFactor)
{

}

void skeleton_animation_update_bone(const csSkeletonAnimation::Channel &channel,
                                    csSkeleton::Bone &bone,
                                    float frame,
                                    float blendFactor)
{
  skeleton_animation_update_bone_rotation(channel, bone, frame, blendFactor);
  skeleton_animation_update_bone_position(channel, bone, frame, blendFactor);
  skeleton_animation_update_bone_scale(channel, bone, frame, blendFactor);
}


void csSkeletonAnimation::PushSkeleton(cryo::csSkeleton *skeleton, float frame, float blendFactor) const
{
  for (const auto &channel: m_channels)
  {
    csSkeleton::Bone &bone = skeleton->GetBone(skeleton->IndexOf(channel.name));
    cryo::skeleton_animation_update_bone(channel, bone, frame, blendFactor);
  }
}

void csSkeletonAnimation::AddRotationFrame(const std::string &channelName,
                                           float frame,
                                           const csQuaternion &rotation)
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

void csSkeletonAnimation::AddPositionFrame(const std::string &channelName, float frame, const cryo::csVector3f &position)
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


void csSkeletonAnimation::AddScaleFrame(const std::string &channelName, float time, const cryo::csVector3f &scale)
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


csSkeletonAnimation::Channel &csSkeletonAnimation::GetChannel(const std::string &channelName)
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


void csSkeletonAnimationPack::AddAnimation(csSkeletonAnimation *animation)
{
  if (animation)
  {
    CS_ADDREF(animation);
    m_animations.push_back(animation);
  }
}

csSkeletonAnimation *csSkeletonAnimationPack::Get(const std::string &name)
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