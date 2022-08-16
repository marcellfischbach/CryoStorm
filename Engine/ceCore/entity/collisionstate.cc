#include <ceCore/entity/collisionstate.hh>
#include <ceCore/physics/iphysicssystem.hh>
#include <ceCore/physics/icollider.hh>
#include <ceCore/physics/icollisionshape.hh>

namespace ce
{


CollisionState::CollisionState()
{
  CE_CLASS_GEN_CONSTR;
}

CollisionState::~CollisionState()
{

}


void CollisionState::OnAttachedToWorld(World* world)
{
  // TODO: Get RigidBodyState -> if present don't attach a static body


}

void CollisionState::OnDetachedFromWorld(World* world)
{
  // TODO: Get RigidBodyState -> if present don't dettach a static body

}






SphereColliderState::SphereColliderState()
  : CollisionState()
  , m_radius(1.0f)
{

}

SphereColliderState::~SphereColliderState()
{

}





iCollisionShape* SphereColliderState::CreateShape(iPhysicsSystem* physSystem) const
{
  SphereShapeDesc desc = {};
  desc.Radius = m_radius;

  return physSystem->CreateShape(desc);
}


void SphereColliderState::SetRadius(float radius)
{
  m_radius = radius;
}

float SphereColliderState::GetRadius() const
{
  return m_radius;
}

}
