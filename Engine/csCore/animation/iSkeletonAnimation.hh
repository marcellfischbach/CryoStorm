
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
class CS_CORE_API iSkeletonAnimation : public CS_SUPER(iAsset)
{

public:

  void SetName(const std::string &name);
  const std::string &GetName() const;


  virtual void SetLoop(bool loop) = 0;
  virtual bool IsLoop () const = 0;

  virtual float GetDuration() const = 0;

  virtual void PushSkeleton(csSkeleton *skeleton, float frame, float blendFactor) const = 0;

private:
  std::string m_name;
};

} // cs
