
#include <csCore/entity/csStaticColliderState.hh>
#include <csCore/entity/csCollisionState.hh>
#include <csCore/entity/csWorld.hh>
#include <csCore/physics/iPhysicsSystem.hh>
#include <csCore/physics/iPhysicsWorld.hh>
#include <csCore/physics/iCollider.hh>
#include <csCore/csObjectRegistry.hh>

namespace cs
{


csStaticColliderState::csStaticColliderState(const std::string &name)
: csSpatialState(name)
, m_collider(nullptr)
{
}

csStaticColliderState::~csStaticColliderState()
{
  if (m_collider)
  {
    m_collider = nullptr;
  }
}

void csStaticColliderState::OnAttachedToWorld(csWorld *world)
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


  AcquireEmptyStaticCollider(physSystem);

  for (auto collisionState : collisionStates)
  {
    auto shape = collisionState->GetShape();
    m_shapes.emplace_back(shape);
    m_collider->Attach(shape);
  }


  m_collider->SetTransform(GetGlobalMatrix());
  auto physWorld = world->GetPhysicsWorld();
  physWorld->AddCollider(m_collider);
}

void csStaticColliderState::AcquireEmptyStaticCollider(iPhysicsSystem *physSystem)
{
  if (m_collider)
  {
    DetachCurrentShapes();
  }
  else
  {

    m_collider = physSystem->CreateStaticCollider();
  }
}

void csStaticColliderState::DetachCurrentShapes()
{
  for (auto shape : m_shapes)
  {
    m_collider->Detach(shape);
  }
  m_shapes.clear();
}


void csStaticColliderState::OnDetachedFromWorld(csWorld *world)
{
  if (m_collider)
  {
    iPhysicsWorld* physWorld = world->GetPhysicsWorld();
    DetachCurrentShapes();
    physWorld->RemoveCollider(m_collider);
    m_collider = nullptr;
  }
}


void csStaticColliderState::TransformationUpdatedPreChildren()
{
  if (m_collider)
  {
    m_collider->SetTransform(GetGlobalMatrix());
  }
}


}