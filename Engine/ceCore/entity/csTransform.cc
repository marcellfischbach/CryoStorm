
#include <ceCore/entity/csTransform.hh>
#include <ceCore/entity/csSpatialState.hh>

namespace cryo
{


csTransform::csTransform(csSpatialState *state, const Matrix4f &matrix, const Matrix4f &parentMatrix)
    : m_state(state), m_matrix(matrix), m_translation(0.0f, 0.0f, 0.0f), m_rotation(0.0f, 0.0f, 0.0f, 1.0f), m_scale(
    1.0f,
    1.0f,
    1.0f), m_dirty(true), m_parentMatrix(parentMatrix)
{
  ExtractTRS();
}

void csTransform::ExtractTRS()
{
  m_translation = m_matrix.GetTranslation();
  m_scale       = Vector3f(m_matrix.GetXAxis().Length(),
                           m_matrix.GetYAxis().Length(),
                           m_matrix.GetZAxis().Length());

  Matrix3f rotMat(m_matrix.m00 / m_scale.x, m_matrix.m01 / m_scale.x, m_matrix.m02 / m_scale.x,
                  m_matrix.m10 / m_scale.y, m_matrix.m11 / m_scale.y, m_matrix.m12 / m_scale.y,
                  m_matrix.m20 / m_scale.z, m_matrix.m21 / m_scale.z, m_matrix.m22 / m_scale.z
  );
  m_rotation = Quaternion::FromMatrix(rotMat);
}

csTransform &csTransform::SetTranslation(const Vector3f &translation)
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

const Vector3f &csTransform::GetTranslation() const
{
  return m_translation;
}


csTransform &csTransform::SetRotation(const Quaternion &rotation)
{
  m_rotation = rotation;
  m_dirty    = true;
  return *this;
}

const Quaternion &csTransform::GetRotation() const
{
  return m_rotation;
}


csTransform &csTransform::SetScale(const Vector3f &scale)
{
  m_scale = scale;
  m_dirty = true;
  return *this;
}


const Vector3f &csTransform::GetScalar() const
{
  return m_scale;
}

csTransform &csTransform::LookAt(const Vector3f &at, const Vector3f &up)
{
  Vector3f from = Matrix4f::Transform(m_parentMatrix, m_translation);

  Matrix4f matrix;
  matrix.SetLookAtInv(from, at, up);

  Matrix4f parent = m_parentMatrix;
  parent.Invert();
  m_matrix = matrix * parent;

  ExtractTRS();
  m_dirty = true;
  return *this;
}


const Matrix4f &csTransform::GetMatrix() const
{
  UpdateMatrix();
  return m_matrix;
}


const Matrix4f &csTransform::GetGlobalMatrix() const
{
  UpdateMatrix();
  return m_globalMatrix;
}

Vector3f csTransform::GetForward() const
{
  UpdateMatrix();
  return m_globalMatrix * Vector3f(0, 0, 1);
}


Vector3f csTransform::GetBackward() const
{
  UpdateMatrix();
  return m_globalMatrix * Vector3f(0, 0, -1);
}


Vector3f csTransform::GetLeft() const
{
  UpdateMatrix();
  return m_globalMatrix * Vector3f(-1, 0, 0);
}


Vector3f csTransform::GetRight() const
{
  UpdateMatrix();
  return m_globalMatrix * Vector3f(1, 0, 0);
}


Vector3f csTransform::GetUp() const
{
  UpdateMatrix();
  return m_globalMatrix * Vector3f(0, 1, 0);
}


Vector3f csTransform::GetDown() const
{
  UpdateMatrix();
  return m_globalMatrix * Vector3f(0, -1, 0);
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
