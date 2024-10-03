
#pragma once

#include <ceCore/coreexport.hh>

#include <ceCore/math/quaternion.hh>
#include <ceCore/math/vector3f.hh>

namespace ce
{

class SpatialState;
class CS_CORE_API Transform
{
public:
  Transform(SpatialState *state,
            const Matrix4f &matrix,
            const Matrix4f &parentMatrix = Matrix4f()
            );

  Transform &SetTranslation(const Vector3f &translation);
  Transform &SetTranslation(float x, float y, float z);
  const Vector3f &GetTranslation() const;

  Transform& SetRotation(const Quaternion &rotation);
  const Quaternion &GetRotation () const;

  Transform& SetScale(const Vector3f &scale);
  const Vector3f &GetScalar() const;

  Transform& LookAt(const Vector3f &at, const Vector3f &up = Vector3f(0.0f, 1.0f, 0.0f));

  const Matrix4f &GetMatrix() const;
  const Matrix4f &GetGlobalMatrix() const;

  Vector3f GetForward () const;
  Vector3f GetBackward () const;

  Vector3f GetRight () const;
  Vector3f GetLeft () const;

  Vector3f GetUp () const;
  Vector3f GetDown () const;



  void Finish ();

private:
  void ExtractTRS();
  void UpdateMatrix() const;

  SpatialState *m_state;
  Vector3f m_translation;
  Quaternion m_rotation;
  Vector3f m_scale;

  Matrix4f m_parentMatrix;
  mutable bool m_dirty;
  mutable Matrix4f m_matrix;
  mutable Matrix4f m_globalMatrix;
};

}