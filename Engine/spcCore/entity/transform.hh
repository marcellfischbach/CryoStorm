
#pragma once

#include <spcCore/coreexport.hh>

#include <spcCore/math/quaternion.hh>
#include <spcCore/math/vector3f.hh>

namespace spc
{

class SpatialState;
class SPC_CORE_API Transform
{
public:
  Transform(const Vector3f &translation = Vector3f(0, 0, 0),
            const Quaternion &rotation = Quaternion(0, 0, 0, 1),
            const Vector3f &scale = Vector3f(1, 1, 1));
  Transform(const Matrix4f &matrix);

  Transform &SetTranslation(const Vector3f &translation);
  const Vector3f &GetTranslation() const;

  Transform& SetRotation(const Quaternion &rotation);
  const Quaternion &GetRotation () const;

  Transform& SetScale(const Vector3f &scale);
  const Vector3f &GetScale() const;

  const Matrix4f &GetMatrix() const;

private:
  void ExtractTRS();

  Vector3f m_translation;
  Quaternion m_rotation;
  Vector3f m_scale;

  mutable bool m_dirty;
  mutable Matrix4f m_matrix;
};

}