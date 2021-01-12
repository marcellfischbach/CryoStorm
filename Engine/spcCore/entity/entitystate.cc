#include <spcCore/entity/entitystate.hh>
#include <spcCore/entity/entity.hh>
#include <entity/entitystate.refl.hh>

namespace spc
{

EntityState::EntityState(const std::string &name)
  : iObject()
  , m_name(name)
  , m_entity(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
}

EntityState::~EntityState() noexcept
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
    
    SPC_ADDREF(m_entity);
    SPC_RELEASE(oldEntity);
  }
}

Entity* EntityState::GetEntity()
{
  return m_entity;
}


const Entity* EntityState::GetEntity() const
{
  return m_entity;
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

}
