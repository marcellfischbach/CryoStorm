
#pragma once

#include <spcCore/coreexport.hh>

#include <spcCore/math/quaternion.hh>
#include <spcCore/math/vector3f.hh>

namespace spc
{

class SPC_CORE_API Transform
{
public:
  Transform();

  void SetTranslation(const Vector3f &translation);
  const Vector3f &GetTranslation() const;

  void SetRotation(const Quaternion &rotation);
  const Quaternion &GetRotation () const;

  void SetScale(const Vector3f &scale);
  const Vector3f &GetScale() const;

  const Matrix4f &GetMatrix() const;

private:
  Vector3f m_translation;
  Quaternion m_rotation;
  Vector3f m_scale;

  mutable bool m_dirty;
  mutable Matrix4f m_matrix;
};

}