
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/staticmeshstate.hh>
#include <ceCore/graphics/skeleton.hh>

namespace ce
{

class SkeletonMesh;

CE_CLASS()
class CE_CORE_API SkeletonMeshState : public CE_SUPER(StaticMeshState)
{
  CE_CLASS_GEN;
public:
  SkeletonMeshState();
  ~SkeletonMeshState() override = default;

  void SetMesh (Mesh* mesh) override;

  CE_NODISCARD Skeleton &GetSkeleton ();
  CE_NODISCARD const Skeleton &GetSkeleton () const;

protected:
  GfxMesh *CreateGfxMesh() override;

private:
  Skeleton m_skeleton;
};

}
