#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/csMesh.hh>
#include <csCore/graphics/csSkeleton.hh>

namespace cs
{

CS_CLASS()
class CS_CORE_API csSkeletonMesh : public CS_SUPER(csMesh)
{
CS_CLASS_GEN;
public:
  csSkeletonMesh();
  ~csSkeletonMesh() override = default;


  CS_NODISCARD csSkeleton &GetSkeleton();
  CS_NODISCARD const csSkeleton &GetSkeleton() const;


private:
  csSkeleton m_skeleton;
};

}
