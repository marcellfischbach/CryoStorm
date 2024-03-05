#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/mesh.hh>
#include <ceCore/graphics/skeleton.hh>

namespace ce
{

CE_CLASS()
class CE_CORE_API SkeletonMesh : public CE_SUPER(Mesh)
{
CE_CLASS_GEN;
public:
  SkeletonMesh();
  ~SkeletonMesh() override = default;


  CE_NODISCARD Skeleton &GetSkeleton ();
  CE_NODISCARD const Skeleton &GetSkeleton () const;


private:
  Skeleton m_skeleton;
};

}
