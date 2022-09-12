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
  : SpatialState(name)
  , m_shape(nullptr)
  , m_collider(nullptr)
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

void CollisionState::OnAttachedToWorld(World* world)
{
  RigidBodyState* rigidBodyState = GetState<RigidBodyState>();
  if (rigidBodyState)
  {
    // this collider instance is managed via a rigid body... no need add anything
    return;
  }


  // TODO: Get RigidBodyState -> if present don't attach a static body
  auto physSystem = ObjectRegistry::Get<iPhysicsSystem>();
  if (!physSystem)
  {
    return;
  }

  if (!m_shape)
  {
    auto shape = CreateShape(physSystem);
    if (!shape)
    {
      return;
    }
    CE_SET(m_shape, shape);
  }

  if (!m_collider)
  {
    auto collider = physSystem->CreateStaticCollider();
    if (!collider)
    {
      return;
    }
    CE_SET(m_collider, collider);

    m_collider->Attach(m_shape);
  }

  m_collider->SetTransform(GetGlobalMatrix());
  auto physWorld = world->GetPhysicsWorld();
  physWorld->AddCollider(m_collider);
}



void CollisionState::OnDetachedFromWorld(World* world)
{
  DetachFromWorld(world);
}

void CollisionState::DetachFromWorld(World* world)
{
  if (m_collider)
  {
    world->GetPhysicsWorld()->RemoveCollider(m_collider);
  }
}


void CollisionState::TransformationUpdatedPreChildren()
{
  if (m_collider)
  {
    m_collider->SetTransform(GetGlobalMatrix());
  }


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
