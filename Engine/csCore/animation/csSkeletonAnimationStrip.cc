#include <csCore/animation/csSkeletonAnimationStrip.hh>
#include <csCore/graphics/csSkeleton.hh>



namespace cs
{

csSkeletonAnimationStrip::csSkeletonAnimationStrip()
    : iAsset()
    , m_numberOfFrames(0.0f)
    , m_framesPerSecond(24.0f)
    , m_loop(false)
    , m_channels()
{

}

csSkeletonAnimationStrip::~csSkeletonAnimationStrip()
{
  m_channels.clear();
}

void csSkeletonAnimationStrip::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &csSkeletonAnimationStrip::GetName() const
{
  return m_name;
}

void csSkeletonAnimationStrip::SetNumberOfFrames(float numberOfFrames)
{
  m_numberOfFrames = numberOfFrames;
}

float csSkeletonAnimationStrip::GetNumberOfFrames() const
{
  return m_numberOfFrames;
}

void csSkeletonAnimationStrip::SetFramesPerSecond(float framesPerSecond)
{
  m_framesPerSecond = framesPerSecond;
}

float csSkeletonAnimationStrip::GetFramesPerSecond() const
{
  return m_framesPerSecond;
}

void csSkeletonAnimationStrip::SetLoop(bool loop)
{
  m_loop = loop;
}

bool csSkeletonAnimationStrip::IsLoop() const
{
  return m_loop;
}

float csSkeletonAnimationStrip::GetDuration() const
{
  return m_numberOfFrames / m_framesPerSecond;
}

void skeleton_animation_update_bone_rotation(const csSkeletonAnimationStrip::Channel &channel,
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
      float a = 0.0f;
      if (frameX1.frame != frameX0.frame)
      {
        a = (frame - frameX0.frame) / (frameX1.frame - frameX0.frame);
      }

      csQuaternion quat = csQuaternion::Serp(frameX0.rotation, frameX1.rotation, a);
      bone.rotation += quat * blendFactor;
      return;
    }

    last = &frameX1;
  }

}

void skeleton_animation_update_bone_position(const csSkeletonAnimationStrip::Channel &channel,
                                             csSkeleton::Bone &bone,
                                             float frame,
                                             float blendFactor)
{
  if (channel.positions.empty())
  {
    return;
  }

  auto last = &channel.positions[0];
  for (const auto &frameX1: channel.positions)
  {
    if (frameX1.frame >= frame)
    {
      auto &frameX0 = *last;
      float a = 0.0f;
      if (frameX1.frame != frameX0.frame)
      {
        a = (frame - frameX0.frame) / (frameX1.frame - frameX0.frame);
      }

      csVector3f quat = csVector3f::Lerp(frameX0.position, frameX1.position, a);
      bone.position += quat * blendFactor;
      return;
    }

    last = &frameX1;
  }
}

void skeleton_animation_update_bone_scale(const csSkeletonAnimationStrip::Channel &channel,
                                          csSkeleton::Bone &bone,
                                          float frame,
                                          float blendFactor)
{

}

void skeleton_animation_update_bone(const csSkeletonAnimationStrip::Channel &channel,
                                    csSkeleton::Bone &bone,
                                    float frame,
                                    float blendFactor)
{
  skeleton_animation_update_bone_rotation(channel, bone, frame, blendFactor);
  skeleton_animation_update_bone_position(channel, bone, frame, blendFactor);
  skeleton_animation_update_bone_scale(channel, bone, frame, blendFactor);
}


void csSkeletonAnimationStrip::PushSkeleton(cs::csSkeleton *skeleton, float frame, float blendFactor) const
{
  for (const auto &channel: m_channels)
  {
    csSkeleton::Bone &bone = skeleton->GetBone(skeleton->IndexOf(channel.name));
    cs::skeleton_animation_update_bone(channel, bone, frame, blendFactor);
  }
}

void csSkeletonAnimationStrip::AddRotationFrame(const std::string &channelName,
                                                float frame,
                                                const csQuaternion &rotation)
{
  csQuaternion q = rotation;
  q.Normalize();
  FrameRotation rotFrame {
      frame,
      q,
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

void csSkeletonAnimationStrip::AddPositionFrame(const std::string &channelName, float frame, const cs::csVector3f &position)
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


void csSkeletonAnimationStrip::AddScaleFrame(const std::string &channelName, float frame, const cs::csVector3f &scale)
{
  FrameScale scaleFrame {
      frame,
      scale,
  };

  Channel   &channel = GetChannel(channelName);
  for (auto it       = channel.scales.begin(); it != channel.scales.end(); it++)
  {
    if (it->frame > frame)
    {
      channel.scales.insert(it, scaleFrame);
      return;
    }
  }
  channel.scales.push_back(scaleFrame);
}


csSkeletonAnimationStrip::Channel &csSkeletonAnimationStrip::GetChannel(const std::string &channelName)
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


void csSkeletonAnimationPack::AddAnimation(csSkeletonAnimationStrip *animation)
{
  if (animation)
  {
    m_animations.emplace_back(animation);
  }
}

csSkeletonAnimationStrip *csSkeletonAnimationPack::Get(const std::string &name)
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