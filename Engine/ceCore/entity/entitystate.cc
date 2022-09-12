#include <ceCore/entity/entitystate.hh>
#include <ceCore/entity/entity.hh>
#include <ceCore/entity/world.hh>
#include <entity/entitystate.refl.hh>

namespace ce
{

EntityState::EntityState(const std::string &name)
  : iObject()
  , m_name(name)
  , m_needUpdate(false)
  , m_entity(nullptr)
{
  CE_CLASS_GEN_CONSTR;
}

EntityState::~EntityState()
{

}

void EntityState::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &EntityState::GetName() const
{
  return m_name;
}

void EntityState::SetEntity(Entity *entity)
{
  if (m_entity != entity)
  {
    Entity* oldEntity = m_entity;
    m_entity = entity;
    
    UpdateEntity(oldEntity, m_entity);
    
    CE_ADDREF(m_entity);
    CE_RELEASE(oldEntity);
  }
}

Entity* EntityState::GetEntity()
{
  return m_entity;
}


EntityState* EntityState::GetState(const Class* cls)
{
  return m_entity ? m_entity->GetState(cls) : nullptr;
}

const EntityState* EntityState::GetState(const Class* cls) const
{
  return m_entity ? static_cast<const Entity*>(m_entity)->GetState(cls) : nullptr;
}


std::vector<EntityState*> EntityState::GetStates(const Class* cls)
{
  return m_entity ? m_entity->GetStates(cls) : std::vector<EntityState*>();
}

std::vector<const EntityState*> EntityState::GetStates(const Class* cls) const
{
  return m_entity ? static_cast<const Entity*>(m_entity)->GetStates(cls) : std::vector<const EntityState*>();
}


const Entity* EntityState::GetEntity() const
{
  return m_entity;
}

SpatialState* EntityState::GetRoot()
{
  return m_entity ? m_entity->GetRoot() : nullptr;
}


const SpatialState* EntityState::GetRoot() const
{
  return m_entity ? m_entity->GetRoot() : nullptr;
}

World* EntityState::GetWorld()
{
  return m_entity ? m_entity->GetWorld() : nullptr;
}

const World* EntityState::GetWorld() const
{
  return m_entity ? m_entity->GetWorld() : nullptr;
}

void EntityState::SetNeedUpdate(bool needUpdate)
{
  if (m_needUpdate != needUpdate)
  {
    World* world = GetWorld();
    if (world && m_needUpdate)
    {
      world->DetachUpdateState(this);
    }
    m_needUpdate = needUpdate;
    if (world && m_needUpdate)
    {
      world->AttachUpdateState(this);
    }
  }
}

bool EntityState::IsNeedUpdate() const
{
  return m_needUpdate;
}

void EntityState::UpdateEntity(Entity *oldEntity, Entity *newEntity)
{
  if (oldEntity)
  {
    oldEntity->DeregisterEntityState(this);
  }
  if (newEntity)
  {
    newEntity->RegisterEntityState(this);
  }
}

void EntityState::AttachToWorld(World *world)
{
  if (m_needUpdate)
  {
    world->AttachUpdateState(this);
  }
  OnAttachedToWorld(world);
}

void EntityState::DetachFromWorld(World *world)
{
  if (m_needUpdate)
  {
    world->DetachUpdateState(this);
  }
  OnDetachedFromWorld(world);
}


void EntityState::OnAttachedToWorld(World* world)
{
}

void EntityState::OnDetachedFromWorld(World* world)
{

}

void EntityState::Update(float tpf)
{
  // nothing to be done
}

}
