
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csStaticMeshState.hh>
#include <csCore/graphics/csSkeleton.hh>

namespace cryo
{

class csSkeletonMesh;

CS_CLASS()
class CS_CORE_API csSkeletonMeshState : public CS_SUPER(csStaticMeshState)
{
  CS_CLASS_GEN;
public:
  csSkeletonMeshState();
  ~csSkeletonMeshState() override = default;

  void SetMesh(csMesh* mesh) override;

  CS_NODISCARD csSkeleton &GetSkeleton();
  CS_NODISCARD const csSkeleton &GetSkeleton() const;

protected:
  csGfxMesh *CreateGfxMesh() override;

private:
  csSkeleton m_skeleton;
};

}
