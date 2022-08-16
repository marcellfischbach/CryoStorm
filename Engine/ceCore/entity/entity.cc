#include <ceCore/entity/entity.hh>
#include <ceCore/entity/entitystate.hh>
#include <ceCore/entity/spatialstate.hh>
#include <ceCore/entity/world.hh>

namespace ce
{

Entity::Entity(const std::string &name)
    : iObject()
      , m_name(name)
      , m_rootState(nullptr)
      , m_parent(nullptr)
      , m_world(nullptr)
{
  CE_CLASS_GEN_CONSTR;
}

Entity::~Entity()
{

}

void Entity::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &Entity::GetName() const
{
  return m_name;
}

void Entity::SetWorld(World *world)
{
  if (m_world != world)
  {
    if (m_world)
    {
      for (auto state : m_states)
      {
        state->DetachFromWorld(m_world);
      }
    }
    CE_SET(m_world, world);
    if (m_world)
    {
      for (auto state : m_states)
      {
        state->AttachToWorld(m_world);
      }
    }

    for (auto child : m_children)
    {
      child->SetWorld(world);
    }
  }
}

World *Entity::GetWorld()
{
  return m_world;
}

const World *Entity::GetWorld() const
{
  return m_world;
}


bool Entity::Attach(Entity *entity, SpatialState *parentState)
{
  if (!entity)
  {
    return false;
  }
  if (entity->GetParent() || std::find(m_children.begin(), m_children.end(), entity) != m_children.end())
  {
    return false;
  }
  if (parentState && parentState->GetEntity() != this)
  {
    return false;
  }

  entity->m_parent = this;
  m_children.push_back(entity);
  entity->SetWorld(m_world);

  SpatialState *childRoot = entity->GetRoot();
  if (!parentState)
  {
    parentState = GetAbsolutRoot();
  }
  if (childRoot && parentState)
  {
    parentState->Attach(childRoot);
  }
  entity->AddRef();
  // increment our own reference cound because the parent-to-this relation is set
  AddRef();
  return true;
}

bool Entity::Detach(Entity *entity)
{
  if (!entity)
  {
    return false;
  }
  if (entity->GetParent() != this)
  {
    return false;
  }
  auto it = std::find(m_children.begin(), m_children.end(), entity);
  if (it == m_children.end())
  {
    return false;
  }

  entity->m_parent = nullptr;
  m_children.erase(it);
  entity->SetWorld(nullptr);

  SpatialState *childRoot = entity->GetRoot();
  if (childRoot)
  {
    childRoot->DetachSelf();
  }
  entity->Release();

  // decrement our own reference cound because the parent-to-this relation is reset
  Release();
  return true;
}

Entity *Entity::GetParent()
{
  return m_parent;
}

const Entity *Entity::GetParent() const
{
  return m_parent;
}

Size Entity::GetNumberOfChildren() const
{
  return m_children.size();
}

const Entity *Entity::GetChild(Size idx) const
{
  if (idx >= m_children.size())
  {
    return nullptr;
  }
  return m_children[idx];
}

Entity *Entity::GetChild(Size idx)
{
  return const_cast<Entity *>(static_cast<const Entity *>(this)->GetChild(idx));
}

const EntityState* Entity::GetState(const Class* cls) const
{
  for (auto state : m_states)
  {
    if (cls->IsAssignableFrom(state->GetClass()))
    {
      return state;
    }
  }
  return nullptr;
}

EntityState* Entity::GetState(const Class* cls)
{
  return const_cast<EntityState*>(static_cast<const Entity*>(this)->GetState(cls));
}


bool Entity::Attach(EntityState *entityState)
{
  if (!entityState)
  {
    return false;
  }
  if (entityState->GetEntity() != nullptr)
  {
    return false;
  }

  if (std::find(m_states.begin(), m_states.end(), entityState) != m_states.end())
  {
    return false;
  }

  entityState->SetEntity(this);


  auto spatialState = entityState->Query<SpatialState>();
  if (spatialState && !m_rootState)
  {
    SetRoot(spatialState);
  }


  return true;
}

bool Entity::Detach(EntityState *entityState)
{
  if (!entityState)
  {
    return false;
  }
  if (entityState->GetEntity() != this)
  {
    return false;
  }
  auto it = std::find(m_states.begin(), m_states.end(), entityState);
  if (it == m_states.end())
  {
    return false;
  }

  entityState->SetEntity(nullptr);


  auto spatialState = entityState->Query<SpatialState>();
  if (spatialState == m_rootState)
  {
    SetRoot(nullptr);
  }


  return true;
}

void Entity::RegisterEntityState(EntityState *entityState)
{
  if (!entityState)
  {
    return;
  }
  auto it = std::find(m_states.begin(), m_states.end(), entityState);
  if (it == m_states.end())
  {
    entityState->AddRef();
    m_states.push_back(entityState);
    if (m_world)
    {
      entityState->AttachToWorld(m_world);
    }
  }
}

void Entity::DeregisterEntityState(EntityState *entityState)
{
  if (!entityState)
  {
    return;
  }
  auto it = std::find(m_states.begin(), m_states.end(), entityState);
  if (it != m_states.end())
  {
    if (m_world)
    {
      entityState->DetachFromWorld(m_world);
    }

    entityState->Release();
    m_states.erase(it);
  }
}


void Entity::SetRoot(SpatialState *rootState)
{
  bool replace = m_rootState != nullptr;

  if (m_rootState)
  {
    m_rootState->DetachSelf();
  }
  CE_SET(m_rootState, rootState);
  SpatialState *parentRoot = GetAbsolutParentRoot();
  if (parentRoot)
  {
    parentRoot->Attach(m_rootState);
  }

  if (replace && m_rootState)
  {
    for (Entity *child : m_children)
    {
      SpatialState *childRoot = child->GetRoot();
      if (childRoot)
      {
        childRoot->DetachSelf();
        m_rootState->Attach(childRoot);
      }
    }
  }
}


SpatialState *Entity::GetRoot()
{
  return m_rootState;
}

const SpatialState *Entity::GetRoot() const
{
  return m_rootState;
}

SpatialState *Entity::GetAbsolutRoot()
{
  Entity *entity = this;
  while (entity)
  {
    if (entity->m_rootState)
    {
      return entity->m_rootState;
    }
    entity = entity->GetParent();
  }
  return nullptr;
}

SpatialState *Entity::GetAbsolutParentRoot()
{
  return m_parent ? m_parent->GetAbsolutRoot() : nullptr;
}

}
