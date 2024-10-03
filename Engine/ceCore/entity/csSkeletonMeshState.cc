//
// Created by MCEL on 04.03.2024.
//

#include <ceCore/entity/csSkeletonMeshState.hh>
#include <ceCore/graphics/skeletonmesh.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>


namespace cryo
{


csSkeletonMeshState::csSkeletonMeshState()
: csStaticMeshState()
{

}


void csSkeletonMeshState::SetMesh(cryo::Mesh *mesh)
{
  csStaticMeshState::SetMesh(mesh);

//  auto skeletonMesh = mesh->Query<SkeletonMesh>();
  if (auto skeletonMesh = mesh->Query<SkeletonMesh>())
  {
    // crete the duplicate of the origin skeleton of the SkeletonMesh
    m_skeleton = skeletonMesh->GetSkeleton();
  }
}


csSkeleton& csSkeletonMeshState::GetSkeleton()
{
  return m_skeleton;
}

const csSkeleton& csSkeletonMeshState::GetSkeleton() const
{
  return m_skeleton;
}

GfxMesh *csSkeletonMeshState::CreateGfxMesh()
{
  auto gfxMesh = csStaticMeshState::CreateGfxMesh();
  gfxMesh->SetSkeleton(&m_skeleton);


  return gfxMesh;
}

}
