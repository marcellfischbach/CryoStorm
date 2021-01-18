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
  if (!child->GetEntity())
  {
    child->SetEntity(GetEntity());
  }
  return true;
}

bool SpatialState::DetachSelf()
{
  return m_parent && m_parent->Detach(this);
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

Size SpatialState::GetNumberOfChildren() const
{
  return m_children.size();
}

const SpatialState* SpatialState::GetChild(Size idx) const
{
  if (idx >= m_children.size())
  {
    return nullptr;
  }

  return m_children[idx];
}

SpatialState* SpatialState::GetChild(Size idx)
{
  return const_cast<SpatialState*>(static_cast<const SpatialState*>(this)->GetChild(idx));
}

void SpatialState::UpdateEntity(Entity *oldEntity, Entity *newEntity)
{
  EntityState::UpdateEntity(oldEntity, newEntity);
  for (auto child : m_children)
  {
    child->SetEntity(newEntity);
  }
}

void SpatialState::SetTransform(const Transform &transform)
{
	m_transform = transform;
	SetGlobalMatrixDirty();
}

const Transform &SpatialState::GetTransform() const
{
	return m_transform;
}

const Matrix4f& SpatialState::GetGlobalMatrix() const
{
  if (m_globalMatrixDirty)
  {
    if (m_parent)
    {
      m_globalMatrix = m_parent->GetGlobalMatrix() * m_transform.GetMatrix();
    }
    else
    {
      m_globalMatrix = m_transform.GetMatrix();
    }
    m_globalMatrixDirty = false;
  }
  return m_globalMatrix;
}

void SpatialState::SetGlobalMatrixDirty()
{
  m_globalMatrixDirty = true;
  for (auto it : m_children)
  {
    it->SetGlobalMatrixDirty();
  }

  TransformationUpdated();
}

void SpatialState::TransformationUpdated()
{

}

}