
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csEntityState.hh>

namespace cs
{

class csSkeleton;

CS_CLASS()
class CS_CORE_API csSkeletonState : public CS_SUPER(csEntityState)
{
  CS_CLASS_GEN;
public:
  csSkeletonState() = default;
  ~csSkeletonState() override = default;

  void SetSkeleton (csSkeleton *skeleton);
  csSkeleton *GetSkeleton ();
  const csSkeleton *GetSkeleton () const;

private:

  csRef<csSkeleton> m_skeleton = nullptr;

};

} // cs
