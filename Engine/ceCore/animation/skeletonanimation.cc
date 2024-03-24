#include <ceCore/animation/skeletonanimation.hh>

namespace ce
{

SkeletonAnimation::SkeletonAnimation()
    : m_channels()
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

void SkeletonAnimation::SetDuration(float duration)
{
  m_duration = duration;
}

float SkeletonAnimation::GetDuration() const
{
  return m_duration;
}

void SkeletonAnimation::SetFramesPerSecond(float framesPerSecond)
{
  m_framesPerSecond = framesPerSecond;
}

float SkeletonAnimation::GetFramesPerSecond() const
{
  return m_framesPerSecond;
}

void SkeletonAnimation::AddRotationFrame(const std::string &channelName,
                                         float time,
                                         const Quaternion &rotation)
{
  FrameRotation frame {
      time,
      rotation,
  };

  Channel   &channel = GetChannel(channelName);
  for (auto it       = channel.rotations.begin(); it != channel.rotations.end(); it++)
  {
    if (it->time > time)
    {
      channel.rotations.insert(it, frame);
      return;
    }
  }
  channel.rotations.push_back(frame);
}

void SkeletonAnimation::AddPositionFrame(const std::string &channelName, float time, const ce::Vector3f &position)
{
  FramePosition frame {
      time,
      position,
  };

  Channel   &channel = GetChannel(channelName);
  for (auto it       = channel.positions.begin(); it != channel.positions.end(); it++)
  {
    if (it->time > time)
    {
      channel.positions.insert(it, frame);
      return;
    }
  }
  channel.positions.push_back(frame);
}


void SkeletonAnimation::AddScaleFrame(const std::string &channelName, float time, const ce::Vector3f &scale)
{
  FrameScale frame {
      time,
      scale,
  };

  Channel   &channel = GetChannel(channelName);
  for (auto it       = channel.scales.begin(); it != channel.scales.end(); it++)
  {
    if (it->time > time)

    {
      channel.scales.insert(it, frame);
      return;
    }
  }
  channel.scales.push_back(frame);
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