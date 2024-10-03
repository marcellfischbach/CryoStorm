
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/entity/csStaticMeshState.hh>
#include <ceCore/graphics/skeleton.hh>

namespace cryo
{

class SkeletonMesh;

CS_CLASS()
class CS_CORE_API csSkeletonMeshState : public CS_SUPER(csStaticMeshState)
{
  CS_CLASS_GEN;
public:
  csSkeletonMeshState();
  ~csSkeletonMeshState() override = default;

  void SetMesh (Mesh* mesh) override;

  CS_NODISCARD csSkeleton &GetSkeleton();
  CS_NODISCARD const csSkeleton &GetSkeleton() const;

protected:
  GfxMesh *CreateGfxMesh() override;

private:
  csSkeleton m_skeleton;
};

}
