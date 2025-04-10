//
// Created by MCEL on 04.03.2024.
//

#include <csCore/entity/csSkeletonMeshState.hh>
#include <csCore/entity/csSkeletonState.hh>
#include <csCore/graphics/csSkeletonMesh.hh>
#include <csCore/graphics/scene/csGfxMesh.hh>


namespace cs
{


csSkeletonMeshState::csSkeletonMeshState()
: csStaticMeshState()
{

}


void csSkeletonMeshState::SetMesh(cs::csMesh *mesh)
{
  csStaticMeshState::SetMesh(mesh);
  UpdateSkeletonMesh();
}

void csSkeletonMeshState::SetSkeletonState(cs::csSkeletonState *skeletonState)
{
  m_skeletonState = skeletonState;
  UpdateSkeletonMesh();
}


void csSkeletonMeshState::UpdateSkeletonMesh()
{
  csMesh* mesh = GetMesh();
  csSkeletonMesh *skeletonMesh = csQueryClass<csSkeletonMesh>(mesh);
  if (skeletonMesh && m_skeletonState)
  {
    skeletonMesh->SetSkeleton(m_skeletonState->GetSkeleton());
  }
}



csGfxMesh *csSkeletonMeshState::CreateGfxMesh()
{
  auto gfxMesh = csStaticMeshState::CreateGfxMesh();
  if (m_skeletonState)
  {
    gfxMesh->SetSkeleton(m_skeletonState->GetSkeleton());
  }


  return gfxMesh;
}

}
