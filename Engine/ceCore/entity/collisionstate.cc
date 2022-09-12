#include <ceCore/entity/collisionstate.hh>
#include <ceCore/entity/rigidbodystate.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/physics/iphysicssystem.hh>
#include <ceCore/physics/icollider.hh>
#include <ceCore/physics/icollisionshape.hh>
#include <ceCore/physics/iphysicsworld.hh>
#include <ceCore/objectregistry.hh>

namespace ce
{


CollisionState::CollisionState(const std::string &name)
  : EntityState(name)
  , m_shape(nullptr)
{
}

CollisionState::~CollisionState()
{

}


iCollisionShape* CollisionState::GetShape()
{
  if (!m_shape)
  {
    auto physSystem = ObjectRegistry::Get<iPhysicsSystem>();
    if (physSystem)
    {
      m_shape = CreateShape(physSystem);
    }
  }
  return m_shape;
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









BoxColliderState::BoxColliderState()
  : CollisionState()
  , m_halfExtends(1.0f, 1.0f, 1.0f)
{

}

BoxColliderState::~BoxColliderState()
{

}





iCollisionShape* BoxColliderState::CreateShape(iPhysicsSystem* physSystem) const
{
  BoxShapeDesc desc = {};
  desc.HalfExtents = m_halfExtends;

  return physSystem->CreateShape(desc);
}


void BoxColliderState::SetHalfExtends(const Vector3f &halfExtends)
{
  m_halfExtends = halfExtends;
}

const Vector3f &BoxColliderState::GetHalfExtends() const
{
  return m_halfExtends;
}


}
