
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
  Transform(SpatialState *spatialState);

  Transform &SetTranslation(const Vector3f &translation);
  const Vector3f &GetTranslation() const;

  Transform& SetRotation(const Quaternion &rotation);
  const Quaternion &GetRotation () const;

  Transform& SetScale(const Vector3f &scale);
  const Vector3f &GetScale() const;

  void Finish();

  const Matrix4f &GetMatrix() const;

private:
  SpatialState* m_spatialState;

  Vector3f m_translation;
  Quaternion m_rotation;
  Vector3f m_scale;

  mutable bool m_dirty;
  mutable Matrix4f m_matrix;
};

}