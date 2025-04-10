
#include <csCore/entity/csTransform.hh>
#include <csCore/entity/csSpatialState.hh>

namespace cs
{


csTransform::csTransform(csSpatialState *state, const csMatrix4f &matrix, const csMatrix4f &parentMatrix)
    : m_state(state)
    , m_matrix(matrix)
    , m_translation(0.0f, 0.0f, 0.0f)
    , m_rotation(0.0f, 0.0f, 0.0f, 1.0f)
    , m_scale(1.0f, 1.0f, 1.0f)
    , m_dirty(true)
    , m_parentMatrix(parentMatrix)
{
  ExtractTRS();
}

void csTransform::ExtractTRS()
{
  csMatrix3f rot;
  m_matrix.ExtractTRS(m_translation, rot, m_scale);
  m_rotation = csQuaternion::FromMatrix(rot);
}

csTransform &csTransform::SetTranslation(const csVector3f &translation)
{
  m_translation = translation;
  m_dirty       = true;
  return *this;
}

csTransform &csTransform::SetTranslation(float x, float y, float z)
{
  m_translation.x = x;
  m_translation.y = y;
  m_translation.z = z;
  m_dirty = true;
  return *this;
}

const csVector3f &csTransform::GetTranslation() const
{
  return m_translation;
}


csTransform &csTransform::SetRotation(const csQuaternion &rotation)
{
  m_rotation = rotation;
  m_dirty    = true;
  return *this;
}

const csQuaternion &csTransform::GetRotation() const
{
  return m_rotation;
}


csTransform &csTransform::SetScale(const csVector3f &scale)
{
  m_scale = scale;
  m_dirty = true;
  return *this;
}


const csVector3f &csTransform::GetScalar() const
{
  return m_scale;
}

csTransform &csTransform::LookAt(const csVector3f &at, const csVector3f &up)
{
  csVector3f from = csMatrix4f::Transform(m_parentMatrix, m_translation);

  csMatrix4f matrix;
  matrix.SetLookAtInv(from, at, up);


  csMatrix4f parent = m_parentMatrix;
  parent.Invert();
  m_matrix = matrix * parent;

  ExtractTRS();
  m_dirty = true;
  return *this;
}


const csMatrix4f &csTransform::GetMatrix() const
{
  UpdateMatrix();
  return m_matrix;
}


const csMatrix4f &csTransform::GetGlobalMatrix() const
{
  UpdateMatrix();
  return m_globalMatrix;
}

csVector3f csTransform::GetForward() const
{
  UpdateMatrix();
  return m_globalMatrix * csVector3f(0, 0, 1);
}


csVector3f csTransform::GetBackward() const
{
  UpdateMatrix();
  return m_globalMatrix * csVector3f(0, 0, -1);
}


csVector3f csTransform::GetLeft() const
{
  UpdateMatrix();
  return m_globalMatrix * csVector3f(-1, 0, 0);
}


csVector3f csTransform::GetRight() const
{
  UpdateMatrix();
  return m_globalMatrix * csVector3f(1, 0, 0);
}


csVector3f csTransform::GetUp() const
{
  UpdateMatrix();
  return m_globalMatrix * csVector3f(0, 1, 0);
}


csVector3f csTransform::GetDown() const
{
  UpdateMatrix();
  return m_globalMatrix * csVector3f(0, -1, 0);
}

void csTransform::Finish()
{
  UpdateMatrix();
  m_state->SetLocalMatrix(m_matrix);
}


void csTransform::UpdateMatrix() const
{
  if (m_dirty)
  {
    m_matrix = m_rotation.ToMatrix4();
    m_matrix.SetTranslation(m_translation);
    m_matrix.Scale(m_scale);
    m_globalMatrix = m_parentMatrix * m_matrix;
    m_dirty        = false;
  }

}

}
