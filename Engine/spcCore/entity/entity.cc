#include <spcCore/entity/entity.hh>
#include <spcCore/entity/entitystate.hh>
#include <spcCore/entity/spatialstate.hh>

namespace spc
{

Entity::Entity(const std::string &name)
  : iObject()
  , m_name(name)
  , m_rootState(nullptr)
  , m_parent(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
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

bool Entity::Attach(Entity* entity, SpatialState* parentState)
{
  if (!entity)
  {
    return false;
  }
  if (entity->GetParent())
  {
    return false;
  }
  if (std::find(m_children.begin(), m_children.end(), entity) != m_children.end())
  {
    return false;
  }
  if (parentState && parentState->GetEntity() != this)
  {
    return false;
  }

  entity->m_parent = this;
  m_children.push_back(entity);

  SpatialState* childRoot = entity->GetRoot();
  if (!parentState)
  {
    parentState = m_rootState;
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

bool Entity::Detach(Entity* entity)
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

  SpatialState* childRoot = entity->GetRoot();
  if (childRoot)
  {
    childRoot->DetachSelf();
  }
  entity->Release();

  // decrement our own reference cound because the parent-to-this relation is reset
  Release();
  return true;
}

Entity* Entity::GetParent()
{
  return m_parent;
}

const Entity* Entity::GetParent() const
{
  return m_parent;
}

Size Entity::GetNumberOfChildren() const
{
  return m_children.size();
}

const Entity* Entity::GetChild(Size idx) const
{
  if (idx >= m_children.size())
  {
    return nullptr;
  }
  return m_children[idx];
}

Entity* Entity::GetChild(Size idx)
{
  return const_cast<Entity*>(static_cast<const Entity*>(this)->GetChild(idx));
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
    entityState->Release();
    m_states.erase(it);
  }
}

void Entity::SetRoot(SpatialState *rootState)
{
  SPC_SET(m_rootState, rootState);
}

SpatialState *Entity::GetRoot()
{
  return m_rootState;
}

const SpatialState *Entity::GetRoot() const
{
  return m_rootState;
}

}
