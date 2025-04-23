

#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/entity/csEntityState.hh>
#include <csCore/csRef.hh>


namespace cs
{

class csSkeletonState;
class csSkeletonAnimationStrip;
class csSkeletonAnimationPlayer;

CS_CLASS(jclass="org.cryo.core.entity.CsSkeletonAnimationState")
class CS_CORE_API csSkeletonAnimationState : public CS_SUPER(csEntityState)
{
  CS_CLASS_GEN;
public:
  csSkeletonAnimationState();
  ~csSkeletonAnimationState() override = default;

  void SetSkeleton (csSkeletonState* skeletonState);
  void SetAnimation(csSkeletonAnimationStrip* animation);

  void SetActive (bool active);
  bool IsActive () const;

  void SetInitialStartTime (float startTime);
  float GetInitialStartTime () const;


  void Update(float tpf) override;
private:
  float m_initialStartTime = 0.0f;
  bool m_active = false;

  csRef<csSkeletonState>           m_skeletonState;
  csRef<csSkeletonAnimationStrip>  m_animation;
  csRef<csSkeletonAnimationPlayer> m_player;
};

} // cs
