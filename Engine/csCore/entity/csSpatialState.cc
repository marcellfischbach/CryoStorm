#include <csCore/entity/csSpatialState.hh>
#include <csCore/entity/csSpatialState.refl.cc>

//#define CS_IS_TRANSFORM_FLAG(f)  ((m_transformationState & (f)) == (f))
//#define CS_SET_TRANSFORM_FLAG(f)  m_transformationState |= (f)
//#define CS_UNSET_TRANSFORM_FLAG(f)  m_transformationState &= ~(f)


namespace cs
{
csSpatialState::csSpatialState(const std::string& name)
  : csEntityState(name), m_parent(nullptr), m_static(false)
{

}

csSpatialState::~csSpatialState()
{

}

void csSpatialState::SetStatic(bool _static)
{
  m_static = _static;
}

bool csSpatialState::IsStatic() const
{
  return m_static;
}


bool csSpatialState::AttachSpatial(cs::csSpatialState *child)
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

  m_children.push_back(child);
  child->m_parent = this;
  if (!child->GetEntity())
  {
    child->SetEntity(GetEntity());
  }
  return true;
}

bool csSpatialState::DetachSelf()
{
  return m_parent && m_parent->DetachSpatial(this);
}

bool csSpatialState::DetachSpatial(cs::csSpatialState *child)
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
  return true;
}

csSpatialState *csSpatialState::GetParent()
{
  return m_parent;
}

const csSpatialState *csSpatialState::GetParent() const
{
  return m_parent;
}

Size csSpatialState::GetNumberOfChildren() const
{
  return m_children.size();
}

const csSpatialState *csSpatialState::GetChild(Size idx) const
{
  if (idx >= m_children.size())
  {
    return nullptr;
  }

  return m_children[idx];
}

csSpatialState *csSpatialState::GetChild(Size idx)

{
  if (idx >= m_children.size())
  {
    return nullptr;
  }

  return m_children[idx];
}

void csSpatialState::UpdateEntity(csEntity *oldEntity, csEntity *newEntity)
{
  csEntityState::UpdateEntity(oldEntity, newEntity);
  for (auto child : m_children)
  {
    child->SetEntity(newEntity);
  }
}

csTransform csSpatialState::GetTransform()
{
  if (m_parent)
  {
    return csTransform(this, m_localMatrix, m_parent->GetGlobalMatrix());
  }
  return csTransform(this, m_localMatrix);
}

void csSpatialState::SetTransform(const csTransform& transform)
{
  m_localMatrix = transform.GetMatrix();

  UpdateTransformation();
}


const csMatrix4f& csSpatialState::GetLocalMatrix() const
{
  return m_localMatrix;
}

void csSpatialState::SetLocalMatrix(const csMatrix4f& matrix)
{
  m_localMatrix = matrix;
  UpdateTransformation();
}


void csSpatialState::UpdateGlobalMatrix()
{
  if (m_parent)
  {
    m_globalMatrix = m_parent->GetGlobalMatrix() * m_localMatrix;
  }
  else
  {
    m_globalMatrix = m_localMatrix;
  }


}

void csSpatialState::UpdateTransformation()
{
  UpdateGlobalMatrix();

  TransformationUpdatedPreChildren();
  for (auto child : m_children)
  {
    child->UpdateTransformation();
  }
  TransformationUpdatedPostChildren();
}

const csMatrix4f& csSpatialState::GetGlobalMatrix() const
{
  return m_globalMatrix;
}


void csSpatialState::TransformationUpdatedPreChildren()
{
  // no default implementation for transformation update
}


void csSpatialState::TransformationUpdatedPostChildren()
{
  // no default implementation for transformation update
}

}