#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/mesh.hh>
#include <ceCore/graphics/skeleton.hh>

namespace ce
{

CS_CLASS()
class CS_CORE_API SkeletonMesh : public CS_SUPER(Mesh)
{
CS_CLASS_GEN;
public:
  SkeletonMesh();
  ~SkeletonMesh() override = default;


  CS_NODISCARD Skeleton &GetSkeleton ();
  CS_NODISCARD const Skeleton &GetSkeleton () const;


private:
  Skeleton m_skeleton;
};

}
