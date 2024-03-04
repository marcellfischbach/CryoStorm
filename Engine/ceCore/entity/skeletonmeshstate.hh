
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/staticmeshstate.hh>

namespace ce
{

class Skeleton;

CE_CLASS()
class CE_CORE_API SkeletonMeshState : public CE_SUPER(StaticMeshState)
{
  CE_CLASS_GEN;
public:
  SkeletonMeshState();
  ~SkeletonMeshState() override;

  void SetSkeleton (Skeleton *skeleton);
  CE_NODISCARD Skeleton *GetSkeleton ();
  CE_NODISCARD const Skeleton *GetSkeleton () const;


private:
  Skeleton *m_skeleton;
};

}
