
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/math/vector3f.hh>
#include <ceCore/math/quaternion.hh>

namespace ce
{


CE_CLASS()
class CE_CORE_API SkeletonAnimation : public CE_SUPER(iObject)
{
CE_CLASS_GEN_OBJECT;

public:
  struct FrameRotation
  {
    float     time;
    Quaternion rotation;
  };
  struct FramePosition
  {
    float   time;
    Vector3f position;
  };
  struct FrameScale
  {
    float   time;
    Vector3f scale;
  };


  struct Channel
  {
    std::string                name;
    std::vector<FrameRotation> rotations;
    std::vector<FramePosition> positions;
    std::vector<FrameScale>    scales;
  };


public:
  SkeletonAnimation();
  ~SkeletonAnimation() override;

  void SetName(const std::string &name);
  const std::string &GetName() const;

  void SetDuration(float duration);
  float GetDuration () const;
  
  void SetFramesPerSecond(float framesPerSecond);
  float GetFramesPerSecond () const;
  

  void AddRotationFrame(const std::string &channelName,
                        float time,
                        const Quaternion &rotation);

  void AddPositionFrame(const std::string &channelName,
                        float time,
                        const Vector3f &position);

  void AddScaleFrame(const std::string &channelName,
                        float time,
                        const Vector3f &scale);

private:
  std::string          m_name;
  float                m_duration;
  float                m_framesPerSecond;
  Channel &GetChannel(const std::string &channelName);
  std::vector<Channel> m_channels;
};


CE_CLASS()
class CE_CORE_API SkeletonAnimationPack : public CE_SUPER(iObject)
{
CE_CLASS_GEN_OBJECT;
public:
  SkeletonAnimationPack() = default;
  ~SkeletonAnimationPack() override = default;

  void AddAnimation(SkeletonAnimation *animation);
  SkeletonAnimation *Get(const std::string &name);


private:
  std::vector<SkeletonAnimation *> m_animations;

};

}