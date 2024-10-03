
#pragma once

#include <ceCore/coreexport.hh>

#include <ceCore/math/quaternion.hh>
#include <ceCore/math/vector3f.hh>

namespace cryo
{

class csSpatialState;
class CS_CORE_API csTransform
{
public:
  csTransform(csSpatialState *state,
              const Matrix4f &matrix,
              const Matrix4f &parentMatrix = Matrix4f()
             );

  csTransform &SetTranslation(const Vector3f &translation);
  csTransform &SetTranslation(float x, float y, float z);
  const Vector3f &GetTranslation() const;

  csTransform& SetRotation(const Quaternion &rotation);
  const Quaternion &GetRotation () const;

  csTransform& SetScale(const Vector3f &scale);
  const Vector3f &GetScalar() const;

  csTransform& LookAt(const Vector3f &at, const Vector3f &up = Vector3f(0.0f, 1.0f, 0.0f));

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

  csSpatialState *m_state;
  Vector3f       m_translation;
  Quaternion m_rotation;
  Vector3f m_scale;

  Matrix4f m_parentMatrix;
  mutable bool m_dirty;
  mutable Matrix4f m_matrix;
  mutable Matrix4f m_globalMatrix;
};

}