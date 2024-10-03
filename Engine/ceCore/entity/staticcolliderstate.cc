
#include <ceCore/entity/staticcolliderstate.hh>
#include <ceCore/entity/collisionstate.hh>
#include <ceCore/entity/world.hh>
#include <ceCore/physics/iphysicssystem.hh>
#include <ceCore/physics/iphysicsworld.hh>
#include <ceCore/physics/icollider.hh>
#include <ceCore/objectregistry.hh>

namespace ce
{


StaticColliderState::StaticColliderState(const std::string &name)
: SpatialState(name)
, m_collider(nullptr)
{
}

StaticColliderState::~StaticColliderState()
{
  if (m_collider)
  {
    // TODO: Detach collider from world and delete
    m_collider = nullptr;
  }
}

void StaticColliderState::OnAttachedToWorld(World *world)
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

void StaticColliderState::AcquireEmptyStaticCollider(iPhysicsSystem *physSystem)
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

void StaticColliderState::DetachCurrentShapes()
{
  for (auto shape : m_shapes)
  {
    m_collider->Detach(shape);
  }
  m_shapes.clear();
}


void StaticColliderState::OnDetachedFromWorld(World *world)
{
  if (m_collider)
  {
    iPhysicsWorld* physWorld = world->GetPhysicsWorld();
    DetachCurrentShapes();
    physWorld->RemoveCollider(m_collider);
    CS_RELEASE(m_collider);
    m_collider = nullptr;
  }
}


void StaticColliderState::TransformationUpdatedPreChildren()
{
  if (m_collider)
  {
    m_collider->SetTransform(GetGlobalMatrix());
  }
}


}