#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/mesh.hh>
#include <ceCore/graphics/skeleton.hh>

namespace cryo
{

CS_CLASS()
class CS_CORE_API SkeletonMesh : public CS_SUPER(Mesh)
{
CS_CLASS_GEN;
public:
  SkeletonMesh();
  ~SkeletonMesh() override = default;


  CS_NODISCARD csSkeleton &GetSkeleton();
  CS_NODISCARD const csSkeleton &GetSkeleton() const;


private:
  csSkeleton m_skeleton;
};

}
