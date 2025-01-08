#include "csEntity.hh"
#include <csCore/entity/csEntityState.hh>
#include <csCore/entity/csSpatialState.hh>
#include <csCore/entity/csWorld.hh>

namespace cs
{

csEntity::csEntity(const std::string &name)
    : iObject()
    , m_name(name)
    , m_rootState(nullptr)
    , m_parent(nullptr)
    , m_world(nullptr)
{

}

csEntity::~csEntity()
{

}

void csEntity::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &csEntity::GetName() const
{
  return m_name;
}

void csEntity::SetWorld(csWorld *world)
{
  if (m_world != world)
  {
    if (m_world)
    {
      for (auto state: m_states)
      {
        state->DetachFromWorld(m_world);
      }
    }
    CS_SET(m_world, world);
    if (m_world)
    {
      for (auto state: m_states)
      {
        state->AttachToWorld(m_world);
      }
    }

    for (auto child: m_children)
    {
      child->SetWorld(world);
    }
  }
}

csWorld *csEntity::GetWorld()
{
  return m_world;
}

const csWorld *csEntity::GetWorld() const
{
  return m_world;
}


bool csEntity::AttachEntity(csRef<cs::csEntity> &entity, csRef<csSpatialState> &parentState)
{
  if (!entity)
  {
    return false;
  }
  if (entity->GetParent() || std::ranges::find(m_children.begin(), m_children.end(), entity) != m_children.end())
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

  csRef<csSpatialState> &childRoot = entity->GetRoot();
  if (!parentState)
  {
    parentState = GetAbsolutRoot();
  }
  if (childRoot && parentState)
  {
    parentState->AttachSpatial(childRoot);
  }
  return true;
}

bool csEntity::DetachEntity(csRef<cs::csEntity> &entity)
{
  if (!entity)
  {
    return false;
  }
  if (entity->GetParent() != this)
  {
    return false;
  }
  auto it = std::ranges::find(m_children.begin(), m_children.end(), entity);
  if (it == m_children.end())
  {
    return false;
  }

  entity->m_parent = nullptr;
  m_children.erase(it);
  entity->SetWorld(nullptr);

  csRef<csSpatialState> &childRoot = entity->GetRoot();
  if (childRoot)
  {
    childRoot->DetachSelf();
  }

  return true;
}

csRef<csEntity> &csEntity::GetParent()
{
  return m_parent;
}

const csRef<csEntity> &csEntity::GetParent() const
{
  return m_parent;
}

size_t csEntity::GetNumberOfChildren() const
{
  return m_children.size();
}

const csRef<csEntity> &csEntity::GetChild(size_t idx) const
{
  if (idx >= m_children.size())
  {
    return csRef<csEntity>::Null();
  }
  return m_children[idx];
}

csRef<csEntity> &csEntity::GetChild(size_t idx)
{
  if (idx >= m_children.size())
  {
    return csRef<csEntity>::Null();
  }
  return m_children[idx];
}

const csRef<csEntityState> &csEntity::GetState(const csClass *cls) const
{
  for (auto state: m_states)
  {
    if (cls->IsAssignableFrom(state->GetClass()))
    {
      return state;
    }
  }
  return csRef<csEntityState>::Null();
}

csRef<csEntityState> &csEntity::GetState(const csClass *cls)
{
  for (auto state : m_states)
  {
    if (cls->IsAssignableFrom(state->GetClass()))
    {
      return state;
    }
  }
  return csRef<csEntityState>::Null();
}

//
//std::vector<const csRef<csEntityState>> csEntity::GetStates(const csClass *cls) const
//{
//  std::vector<const csRef<csEntityState>> result;
//  for (auto                          state: m_states)
//  {
//    if (cls->IsAssignableFrom(state->GetClass()))
//    {
//      result.push_back(state);
//    }
//  }
//  return result;
//}

std::vector<csRef<csEntityState>> csEntity::GetStates(const csClass *cls)
{
  std::vector<csRef<csEntityState>> result;
  for (auto                    state: m_states)
  {
    if (cls->IsAssignableFrom(state->GetClass()))
    {
      result.push_back(state);
    }
  }
  return result;
}

bool csEntity::AttachState(csRef<cs::csEntityState> entityState)
{
  if (!entityState)
  {
    return false;
  }
  if (entityState->GetEntity() != nullptr)
  {
    return false;
  }

  if (std::ranges::find(m_states.begin(), m_states.end(), entityState) != m_states.end())
  {
    return false;
  }

  csRef<csEntity> thisRef(this);
  entityState->SetEntity(thisRef);


  auto spatialState = entityState->Query<csSpatialState>();
  if (spatialState && !m_rootState)
  {
    csRef<csSpatialState> spatialRef(spatialState);
    SetRoot(spatialRef);
  }


  return true;
}

bool csEntity::DetachState(csRef<cs::csEntityState> entityState)
{
  if (!entityState)
  {
    return false;
  }
  if (entityState->GetEntity() != this)
  {
    return false;
  }
  auto it = std::ranges::find(m_states.begin(), m_states.end(), entityState);
  if (it == m_states.end())
  {
    return false;
  }

  entityState->SetEntity(csRef<csEntity>::Null());


  auto spatialState = entityState->Query<csSpatialState>();
  if (spatialState == m_rootState)
  {
    SetRoot(csRef<csSpatialState>::Null());
  }


  return true;
}

void csEntity::RegisterEntityState(csRef<csEntityState> &entityState)
{
  if (!entityState)
  {
    return;
  }
  auto it = std::ranges::find(m_states.begin(), m_states.end(), entityState);
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

void csEntity::DeregisterEntityState(csRef<csEntityState> &entityState)
{
  if (!entityState)
  {
    return;
  }
  auto it = std::ranges::find(m_states.begin(), m_states.end(), entityState);
  if (it != m_states.end())
  {
    if (m_world)
    {
      entityState->DetachFromWorld(m_world);
    }

    m_states.erase(it);
  }
}


void csEntity::SetRoot(csRef<csSpatialState> &rootState)
{
  bool replace = m_rootState != nullptr;

  if (m_rootState)
  {
    m_rootState->DetachSelf();
  }
  m_rootState = rootState;
  csRef<csSpatialState> &parentRoot = GetAbsolutParentRoot();
  if (parentRoot)
  {
    parentRoot->AttachSpatial(rootState);
  }

  if (replace && m_rootState)
  {
    for (csRef<csEntity> &child : m_children)
    {
      csRef<csSpatialState> &childRoot = child->GetRoot();
      if (childRoot)
      {
        childRoot->DetachSelf();
        m_rootState->AttachSpatial(childRoot);
      }
    }
  }
}


csRef<csSpatialState> &csEntity::GetRoot()
{
  return m_rootState;
}

const csRef<csSpatialState> &csEntity::GetRoot() const
{
  return m_rootState;
}

csRef<csSpatialState> &csEntity::GetAbsolutRoot()
{
  csEntity *entity = this;
  while (entity)
  {
    if (entity->m_rootState)
    {
      return entity->m_rootState;
    }
    entity = entity->GetParent().raw();
  }
  return csRef<csSpatialState>::Null();
}

csRef<csSpatialState> &csEntity::GetAbsolutParentRoot()
{
  return m_parent ? m_parent->GetAbsolutRoot() : csRef<csSpatialState>::Null();
}

}


