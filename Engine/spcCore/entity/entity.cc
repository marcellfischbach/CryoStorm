#include <spcCore/entity/entity.hh>
#include <spcCore/entity/entitystate.hh>
#include <spcCore/entity/spatialstate.hh>

namespace spc
{

Entity::Entity(const std::string &name)
  : iObject()
  , m_name(name)
  , m_rootState(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
}

Entity::~Entity() noexcept
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
