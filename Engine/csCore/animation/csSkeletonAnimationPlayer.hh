//
// Created by Marcell on 24.03.2024.
//

#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>

namespace cs
{

class csSkeleton;
class csSkeletonAnimationStrip;

CS_CLASS()
class CS_CORE_API csSkeletonAnimationPlayer : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
public:
  csSkeletonAnimationPlayer();
  ~csSkeletonAnimationPlayer() override;

  void SetSkeleton(csSkeleton *skeleton);
  void SetAnimation(csSkeletonAnimationStrip *animation);

  void Start (float time = 0.0f);

  void Update (float tpf);

private:
  void UpdateFrame(float time);

private:
  csRef<csSkeleton>               m_skeleton;
  csRef<csSkeletonAnimationStrip> m_animation;

  bool m_active;
  float m_time;


};

} // ce
