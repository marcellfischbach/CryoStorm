//
// Created by MCEL on 04.03.2024.
//

#include <ceCore/entity/csSkeletonMeshState.hh>
#include <ceCore/graphics/csSkeletonMesh.hh>
#include <ceCore/graphics/scene/csGfxMesh.hh>


namespace cryo
{


csSkeletonMeshState::csSkeletonMeshState()
: csStaticMeshState()
{

}


void csSkeletonMeshState::SetMesh(cryo::csMesh *mesh)
{
  csStaticMeshState::SetMesh(mesh);

//  auto skeletonMesh = mesh->Query<SkeletonMesh>();
  if (auto skeletonMesh = mesh->Query<csSkeletonMesh>())
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

csGfxMesh *csSkeletonMeshState::CreateGfxMesh()
{
  auto gfxMesh = csStaticMeshState::CreateGfxMesh();
  gfxMesh->SetSkeleton(&m_skeleton);


  return gfxMesh;
}

}
