
#include <csCore/entity/csRigidBodyState.hh>
#include <csCore/entity/csCollisionState.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/physics/iCollider.hh>
#include <csCore/physics/iPhysicsSystem.hh>
#include <csCore/physics/iPhysicsWorld.hh>
#include <csCore/csObjectRegistry.hh>

namespace cs
{


csRigidBodyState::csRigidBodyState(const std::string& name)
  : csSpatialState(name)
  , m_dynamicCollider(nullptr)
{
}


csRigidBodyState::~csRigidBodyState()
{
  // TODO: Detach ridid
}


void csRigidBodyState::OnAttachedToWorld(csWorld* world)
{
  auto physSystem = csObjectRegistry::Get<iPhysicsSystem>();
  if (!physSystem)
  {
    return;
  }

  auto collisionStates = GetStates<csCollisionState>();
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

void csRigidBodyState::AcquireEmptyDynamicCollider(iPhysicsSystem *physSystem)
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


void csRigidBodyState::DetachCurrentShapes()
{
  for (auto shape : m_shapes)
  {
    m_dynamicCollider->Detach(shape);
  }
  m_shapes.clear();
}


void csRigidBodyState::OnDetachedFromWorld(csWorld* world)
{
  if (m_dynamicCollider)
  {
    iPhysicsWorld* physWorld = world->GetPhysicsWorld();
    DetachCurrentShapes();
    m_dynamicCollider->SetUserData(nullptr);
    physWorld->RemoveCollider(m_dynamicCollider);
    CS_RELEASE(m_dynamicCollider);
    m_dynamicCollider = nullptr;
  }
}


void csRigidBodyState::TransformationUpdatedPreChildren()
{
}
}