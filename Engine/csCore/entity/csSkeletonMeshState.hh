
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csStaticMeshState.hh>
#include <csCore/graphics/csSkeleton.hh>

namespace cs
{

class csSkeletonState;
class csSkeletonMesh;

CS_CLASS()
class CS_CORE_API csSkeletonMeshState : public CS_SUPER(csStaticMeshState)
{
  CS_CLASS_GEN;
public:
  csSkeletonMeshState();
  ~csSkeletonMeshState() override = default;

  void SetMesh(csMesh* mesh) override;
  void SetSkeletonState (csSkeletonState *skeletonState);


protected:
  csGfxMesh *CreateGfxMesh() override;

private:
  void UpdateSkeletonMesh ();

private:
  csRef<csSkeletonState> m_skeletonState = nullptr;
};

}
