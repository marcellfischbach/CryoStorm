
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
#include <csCore/math/csVector3f.hh>
#include <csCore/math/csQuaternion.hh>

namespace cs
{

class csSkeleton;
class csSkeletonAnimationProperties;

CS_CLASS()
class CS_CORE_API iSkeletonAnimation : public CS_SUPER(iAsset)
{
  CS_CLASS_GEN;
public:

  void SetName(const std::string &name);
  const std::string &GetName() const;


  virtual void SetLoop(bool loop) = 0;
  virtual bool IsLoop () const = 0;

  virtual float GetDuration(csSkeletonAnimationProperties *properties) const = 0;

  virtual void PushSkeleton(csSkeleton *skeleton, csSkeletonAnimationProperties *properties, float frame, float blendFactor) const = 0;

private:
  std::string m_name;
};

} // cs
