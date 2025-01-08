#include <csCore/entity/csEntityState.hh>
#include "csEntity.hh"
#include <csCore/entity/csWorld.hh>

namespace cs
{

csEntityState::csEntityState(const std::string &name)
  : iObject()
  , m_name(name)
  , m_needUpdate(false)
  , m_entity(nullptr)
{

}


void csEntityState::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &csEntityState::GetName() const
{
  return m_name;
}

void csEntityState::SetEntity(csRef<csEntity> &entity)
{
  if (m_entity != entity)
  {
    csRef<csEntity> oldEntity = m_entity;
    m_entity = entity;
    
    UpdateEntity(oldEntity, m_entity);
  }
}

csRef<csEntity> &csEntityState::GetEntity()
{
  return m_entity;
}


csRef<csEntityState> &csEntityState::GetState(const csClass* cls)
{
  return m_entity ? m_entity->GetState(cls) : csRef<csEntityState>::Null();
}

const csRef<csEntityState> &csEntityState::GetState(const csClass* cls) const
{
  return m_entity ? m_entity->GetState(cls) : csRef<csEntityState>::Null();
}


std::vector<csRef<csEntityState>> csEntityState::GetStates(const csClass* cls)
{
  return m_entity ? m_entity->GetStates(cls) : std::vector<csRef<csEntityState>>();
}

//std::vector<const csRef<csEntityState>> csEntityState::GetStates(const csClass* cls) const
//{
//  return m_entity ? m_entity->GetStates(cls) : std::vector<const csRef<csEntityState>>();
//}


const csRef<csEntity> &csEntityState::GetEntity() const
{
  return m_entity;
}

csRef<csSpatialState> &csEntityState::GetRoot()
{
  return m_entity ? m_entity->GetRoot() : csRef<csSpatialState>::Null();
}


const csRef<csSpatialState> &csEntityState::GetRoot() const
{
  return m_entity ? m_entity->GetRoot() : csRef<csSpatialState>::Null();
}

csWorld* csEntityState::GetWorld()
{
  return m_entity ? m_entity->GetWorld() : nullptr;
}

const csWorld* csEntityState::GetWorld() const
{
  return m_entity ? m_entity->GetWorld() : nullptr;
}

void csEntityState::SetNeedUpdate(bool needUpdate)
{
  if (m_needUpdate != needUpdate)
  {
    csWorld * world = GetWorld();
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

bool csEntityState::IsNeedUpdate() const
{
  return m_needUpdate;
}

void csEntityState::UpdateEntity(csRef<csEntity> &oldEntity, csRef<csEntity> &newEntity)
{
  csRef<csEntityState> thisState(this);
  if (oldEntity)
  {
    oldEntity->DeregisterEntityState(thisState);
  }
  if (newEntity)
  {
    newEntity->RegisterEntityState(thisState);
  }
}

void csEntityState::AttachToWorld(csWorld *world)
{
  if (m_needUpdate)
  {
    world->AttachUpdateState(this);
  }
  OnAttachedToWorld(world);
}

void csEntityState::DetachFromWorld(csWorld *world)
{
  if (m_needUpdate)
  {
    world->DetachUpdateState(this);
  }
  OnDetachedFromWorld(world);
}


void csEntityState::OnAttachedToWorld(csWorld* world)
{
}

void csEntityState::OnDetachedFromWorld(csWorld* world)
{

}

void csEntityState::Update(float tpf)
{
  // nothing to be done
}

}
