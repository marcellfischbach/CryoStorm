//
// Created by MCEL on 04.03.2024.
//

#include <ceCore/entity/skeletonmeshstate.hh>
#include <ceCore/graphics/skeletonmesh.hh>


namespace ce
{


SkeletonMeshState::SkeletonMeshState()
: StaticMeshState()
{

}


void SkeletonMeshState::SetMesh(ce::Mesh *mesh)
{
  StaticMeshState::SetMesh(mesh);

//  auto skeletonMesh = mesh->Query<SkeletonMesh>();
  if (auto skeletonMesh = mesh->Query<SkeletonMesh>())
  {
    // crete the duplicate of the origin skeleton of the SkeletonMesh
    m_skeleton = skeletonMesh->GetSkeleton();
  }
}


Skeleton& SkeletonMeshState::GetSkeleton()
{
  return m_skeleton;
}

const Skeleton& SkeletonMeshState::GetSkeleton() const
{
  return m_skeleton;
}

GfxMesh *SkeletonMeshState::CreateGfxMesh()
{
  auto gfxMesh = StaticMeshState::CreateGfxMesh();



  return gfxMesh;
}

}
