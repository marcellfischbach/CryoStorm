//
// Created by Marcell on 24.03.2024.
//

#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/csClass.hh>

namespace cryo
{

class csSkeleton;
class csSkeletonAnimation;

CS_CLASS()
class CS_CORE_API csSkeletonAnimationPlayer : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csSkeletonAnimationPlayer();
  ~csSkeletonAnimationPlayer() override;

  void SetSkeleton(csSkeleton *skeleton);
  void SetAnimation(csSkeletonAnimation *animation);

  void Start (float time = 0.0f);

  void Update (float tpf);

private:
  void UpdateFrame(float time);

private:
  csSkeleton          *m_skeleton;
  csSkeletonAnimation *m_animation;

  bool m_active;
  float m_time;


};

} // ce
