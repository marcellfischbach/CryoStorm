
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/staticmeshstate.hh>
#include <ceCore/graphics/skeleton.hh>

namespace ce
{

class SkeletonMesh;

CS_CLASS()
class CS_CORE_API SkeletonMeshState : public CS_SUPER(StaticMeshState)
{
  CS_CLASS_GEN;
public:
  SkeletonMeshState();
  ~SkeletonMeshState() override = default;

  void SetMesh (Mesh* mesh) override;

  CS_NODISCARD Skeleton &GetSkeleton ();
  CS_NODISCARD const Skeleton &GetSkeleton () const;

protected:
  GfxMesh *CreateGfxMesh() override;

private:
  Skeleton m_skeleton;
};

}
