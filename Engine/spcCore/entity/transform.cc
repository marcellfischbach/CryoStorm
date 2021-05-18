
#include <spcCore/entity/transform.hh>
#include <spcCore/entity/spatialstate.hh>

namespace spc
{

Transform::Transform(const Vector3f &translation,
                     const Quaternion &rotation,
                     const Vector3f &scale)
                     : m_translation(translation)
                     , m_rotation(rotation)
                     , m_scale(scale)
{
  m_dirty = true;
}

Transform::Transform(const Matrix4f &matrix)
    : m_matrix(matrix)
      , m_translation(0.0f, 0.0f, 0.0f)
      , m_rotation(0.0f, 0.0f, 0.0f, 1.0f)
      , m_scale(1.0f, 1.0f, 1.0f)
      , m_dirty(false)
{

  ExtractTRS();


}

void Transform::ExtractTRS()
{
  m_translation = m_matrix.GetTranslation();
  m_scale = Vector3f(m_matrix.GetXAxis().Length(),
                     m_matrix.GetYAxis().Length(),
                     m_matrix.GetZAxis().Length());

#define m m_matrix
  Matrix3f rotMat(m.m00 / m_scale.x, m.m01 / m_scale.x, m.m02 / m_scale.x,
                  m.m10 / m_scale.y, m.m11 / m_scale.y, m.m12 / m_scale.y,
                  m.m20 / m_scale.z, m.m21 / m_scale.z, m.m22 / m_scale.z
  );
#undef m
  m_rotation = Quaternion::FromMatrix(rotMat);
}

Transform &Transform::SetTranslation(const Vector3f &translation)
{
  m_translation = translation;
  m_dirty = true;
  return *this;
}

const Vector3f &Transform::GetTranslation() const
{
  return m_translation;
}


Transform &Transform::SetRotation(const Quaternion &rotation)
{
  m_rotation = rotation;
  m_dirty = true;
  return *this;
}

const Quaternion &Transform::GetRotation() const
{
  return m_rotation;
}


Transform &Transform::SetScale(const Vector3f &scale)
{
  m_scale = scale;
  m_dirty = true;
  return *this;
}


const Vector3f &Transform::GetScale() const
{
  return m_scale;
}


const Matrix4f &Transform::GetMatrix() const
{
  if (m_dirty)
  {
    m_matrix = m_rotation.ToMatrix4();
    m_matrix.SetTranslation(m_translation);
    m_matrix.Scale(m_scale);
    m_dirty = false;
  }

  return m_matrix;
}

}
