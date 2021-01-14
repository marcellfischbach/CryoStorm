#include <spcCore/entity/spatialstate.hh>


namespace spc
{
SpatialState::SpatialState(const std::string& name)
  : EntityState(name)
  , m_parent(nullptr)
{

}

SpatialState::~SpatialState() 
{

}

bool SpatialState::Attach(SpatialState *child)
{
  if (!child)
  {
    return false;
  }
  if (child->m_parent)
  {
    return false;
  }
  if (std::find(m_children.begin(), m_children.end(), child) != m_children.end())
  {
    return false;
  }
  
  child->AddRef();
  m_children.push_back(child);
  child->m_parent = this;
  child->SetEntity(GetEntity());
  return true;
}

bool SpatialState::Detach(SpatialState *child)
{
  if (!child)
  {
    return false;
  }
  if (child->m_parent != this)
  {
    return false;
  }
  auto it = std::find(m_children.begin(), m_children.end(), child);
  if (it == m_children.end())
  {
    return false;
  }
  
  m_children.erase(it);
  child->m_parent = nullptr;
  child->Release();
  return true;
}

SpatialState* SpatialState::GetParent()
{
  return m_parent;
}

const SpatialState* SpatialState::GetParent() const
{
  return m_parent;
}

void SpatialState::UpdateEntity(Entity *oldEntity, Entity *newEntity)
{
  EntityState::UpdateEntity(oldEntity, newEntity);
  for (auto child : m_children)
  {
    child->SetEntity(newEntity);
  }
}

}