#include <spcCore/entity/spatialstate.hh>

#define SPC_IS_TRANSFORM_FLAG(f)  ((m_transformationState & (f)) == (f))
#define SPC_SET_TRANSFORM_FLAG(f)  m_transformationState |= (f)
#define SPC_UNSET_TRANSFORM_FLAG(f)  m_transformationState &= ~(f)


namespace spc
{
SpatialState::SpatialState(const std::string& name)
        : EntityState(name), m_transform(this), m_transformationState(0), m_parent(nullptr), m_static(false)
{

}

SpatialState::~SpatialState()
{

}

void SpatialState::SetStatic(bool _static)
{
  m_static = _static;
}

bool SpatialState::IsStatic() const
{
  return m_static;
}

bool SpatialState::Attach(SpatialState* child)
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

bool SpatialState::Detach(SpatialState* child)
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

void SpatialState::UpdateEntity(Entity* oldEntity, Entity* newEntity)
{
  EntityState::UpdateEntity(oldEntity, newEntity);
  for (auto child : m_children)
  {
    child->SetEntity(newEntity);
  }
}

Transform& SpatialState::GetTransform()
{
  return m_transform;
}

const Transform& SpatialState::GetTransform() const
{
  return m_transform;
}

void SpatialState::FinishTransformation()
{
  UpdateFlagRequestHierarchyTransformationUpdate();
  UpdateFlagGlobalMatrixDirty();
}

void SpatialState::UpdateFlagGlobalMatrixDirty()
{
  if (SPC_IS_TRANSFORM_FLAG(eTS_GlobalMatrixDirty))
  {
    return;
  }
  SPC_SET_TRANSFORM_FLAG(eTS_GlobalMatrixDirty);
  SPC_SET_TRANSFORM_FLAG(eTS_RequestHierarchyTransformationUpdate);
  for (auto child : m_children)
  {
    child->UpdateFlagGlobalMatrixDirty();
  }
}

void SpatialState::UpdateFlagRequestHierarchyTransformationUpdate()
{
  if (!SPC_IS_TRANSFORM_FLAG(eTS_RequestHierarchyTransformationUpdate))
  {
    SPC_SET_TRANSFORM_FLAG(eTS_RequestHierarchyTransformationUpdate);
    if (m_parent)
    {
      m_parent->UpdateFlagRequestHierarchyTransformationUpdate();
    }
  }
}

const Matrix4f& SpatialState::GetGlobalMatrix() const
{
  if (SPC_IS_TRANSFORM_FLAG(eTS_GlobalMatrixDirty))
  {
    UpdateGlobalMatrix();
    SPC_UNSET_TRANSFORM_FLAG(eTS_GlobalMatrixDirty);
  }
  return m_globalMatrix;
}

void SpatialState::UpdateGlobalMatrix() const
{
  if (m_parent)
  {
    m_globalMatrix = m_parent->GetGlobalMatrix() * m_transform.GetMatrix();
  }
  else
  {
    m_globalMatrix = m_transform.GetMatrix();
  }
}

void SpatialState::UpdateTransformation()
{
  if (SPC_IS_TRANSFORM_FLAG(eTS_RequestHierarchyTransformationUpdate))
  {

    // calling get global matrix will update the global matrix;
    if (SPC_IS_TRANSFORM_FLAG(eTS_GlobalMatrixDirty))
    {
      UpdateGlobalMatrix();
    }
    m_transformationState = 0;
  
    TransformationUpdatedPreChildren();
    for (auto child : m_children)
    {
      child->UpdateTransformation();
    }
    TransformationUpdatedPostChildren();
  }
}

void SpatialState::TransformationUpdatedPreChildren()
{

}


void SpatialState::TransformationUpdatedPostChildren()
{

}
}