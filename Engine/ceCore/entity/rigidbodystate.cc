
#include <ceCore/entity/rigidbodystate.hh>
#include <ceCore/entity/collisionstate.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/physics/icollider.hh>
#include <ceCore/physics/iphysicssystem.hh>
#include <ceCore/physics/iphysicsworld.hh>
#include <ceCore/objectregistry.hh>

namespace ce
{


RigidBodyState::RigidBodyState(const std::string& name)
  : SpatialState(name)
  , m_dynamicCollider(nullptr)
{
}


RigidBodyState::~RigidBodyState()
{
}


void RigidBodyState::OnAttachedToWorld(World* world)
{
  auto physSystem = ObjectRegistry::Get<iPhysicsSystem>();
  iPhysicsWorld* physWorld = world->GetPhysicsWorld();
  if (!physSystem || !physWorld)
  {
    return;
  }


  std::vector<CollisionState*> collisionStates = GetStates<CollisionState>();
  if (collisionStates.empty())
  {
    return;
  }

  DetachCollider (physWorld);

  m_dynamicCollider = physSystem->CreateDynamicCollider();
  for (auto collisionState : collisionStates)
  {
    // if the collision states where added to the world before the rigid body was, then we must detach the static
    // colliders first
    collisionState->DetachFromWorld(world);
    m_dynamicCollider->Attach(collisionState->GetShape());
  }

  AttachCollider (physWorld);
}

void RigidBodyState::OnDetachedFromWorld(World* world)
{
  iPhysicsWorld* physWorld = world->GetPhysicsWorld();
  DetachCollider(physWorld);
}


void RigidBodyState::AttachCollider(iPhysicsWorld* physWorld)
{
  if (m_dynamicCollider)
  {
    physWorld->AddCollider(m_dynamicCollider);
  }
}

void RigidBodyState::DetachCollider(iPhysicsWorld* physWorld)
{
  if (m_dynamicCollider)
  {
    physWorld->RemoveCollider(m_dynamicCollider);
    CE_RELEASE(m_dynamicCollider);
    m_dynamicCollider = nullptr;
  }
}


void RigidBodyState::TransformationUpdatedPreChildren()
{
}
}