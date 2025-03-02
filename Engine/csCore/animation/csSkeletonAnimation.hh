
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
#include <csCore/math/csVector3f.hh>
#include <csCore/math/csQuaternion.hh>


namespace cs
{

class csSkeleton;

CS_CLASS()
class CS_CORE_API csSkeletonAnimation : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;

public:
  struct FrameRotation
  {
    float        frame;
    csQuaternion rotation;
  };
  struct FramePosition
  {
    float      frame;
    csVector3f position;
  };
  struct FrameScale
  {
    float      frame;
    csVector3f scale;
  };


  struct Channel
  {
    std::string                name;
    std::vector<FrameRotation> rotations;
    std::vector<FramePosition> positions;
    std::vector<FrameScale>    scales;
  };


public:
  csSkeletonAnimation();
  ~csSkeletonAnimation() override;

  void SetName(const std::string &name);
  const std::string &GetName() const;

  void SetNumberOfFrames(float numberOfFrames);
  float GetNumberOfFrames() const;
  
  void SetFramesPerSecond(float framesPerSecond);
  float GetFramesPerSecond () const;

  void SetLoop(bool loop);
  bool IsLoop () const;

  float GetDuration() const;


  void PushSkeleton(cs::csSkeleton *skeleton, float frame, float blendFactor) const;

  void AddRotationFrame(const std::string &channelName,
                        float frame,
                        const csQuaternion &rotation);

  void AddPositionFrame(const std::string &channelName,
                        float frame,
                        const csVector3f &position);

  void AddScaleFrame(const std::string &channelName,
                        float time,
                        const csVector3f &scale);

private:

  std::string m_name;
  float       m_numberOfFrames;
  float       m_framesPerSecond;
  bool        m_loop;
  Channel &GetChannel(const std::string &channelName);
  std::vector<Channel> m_channels;
};


CS_CLASS()
class CS_CORE_API csSkeletonAnimationPack : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  csSkeletonAnimationPack() = default;
  ~csSkeletonAnimationPack() override = default;

  void AddAnimation(csSkeletonAnimation *animation);
  csSkeletonAnimation *Get(const std::string &name);


private:
  std::vector<csRef<csSkeletonAnimation>> m_animations;

};

}