
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
  // TODO: Detach ridid
}


void RigidBodyState::OnAttachedToWorld(World* world)
{
  auto physSystem = ObjectRegistry::Get<iPhysicsSystem>();
  if (!physSystem)
  {
    return;
  }

  auto collisionStates = GetStates<CollisionState>();
  if (collisionStates.empty())
  {
    return;
  }


  AcquireEmptyDynamicCollider(physSystem);

  for (auto collisionState : collisionStates)
  {
    auto shape = collisionState->GetShape();
    m_shapes.emplace_back(shape);
    m_dynamicCollider->Attach(shape);
  }


  m_dynamicCollider->SetTransform(GetGlobalMatrix());
  auto physWorld = world->GetPhysicsWorld();
  physWorld->AddCollider(m_dynamicCollider);
}

void RigidBodyState::AcquireEmptyDynamicCollider(iPhysicsSystem *physSystem)
{
  if (m_dynamicCollider)
  {
    DetachCurrentShapes();
  }
  else
  {
    m_dynamicCollider = physSystem->CreateDynamicCollider();
    m_dynamicCollider->SetUserData(this);
  }
}


void RigidBodyState::DetachCurrentShapes()
{
  for (auto shape : m_shapes)
  {
    m_dynamicCollider->Detach(shape);
  }
  m_shapes.clear();
}


void RigidBodyState::OnDetachedFromWorld(World* world)
{
  if (m_dynamicCollider)
  {
    iPhysicsWorld* physWorld = world->GetPhysicsWorld();
    DetachCurrentShapes();
    m_dynamicCollider->SetUserData(nullptr);
    physWorld->RemoveCollider(m_dynamicCollider);
    CE_RELEASE(m_dynamicCollider);
    m_dynamicCollider = nullptr;
  }
}


void RigidBodyState::TransformationUpdatedPreChildren()
{
}
}