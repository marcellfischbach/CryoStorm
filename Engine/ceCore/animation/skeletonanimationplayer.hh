//
// Created by Marcell on 24.03.2024.
//

#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>

namespace ce
{

class Skeleton;
class SkeletonAnimation;

CS_CLASS()
class CS_CORE_API SkeletonAnimationPlayer : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  SkeletonAnimationPlayer();
  ~SkeletonAnimationPlayer() override;

  void SetSkeleton(Skeleton *skeleton);
  void SetAnimation (SkeletonAnimation *animation);

  void Start (float time = 0.0f);

  void Update (float tpf);

private:
  void UpdateFrame(float time);

private:
  Skeleton *m_skeleton;
  SkeletonAnimation *m_animation;

  bool m_active;
  float m_time;


};

} // ce
