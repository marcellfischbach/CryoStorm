
#pragma once

#include <csCore/csCoreExport.hh>

#include <csCore/math/csQuaternion.hh>
#include <csCore/math/csVector3f.hh>

namespace cs
{

class csSpatialState;
class CS_CORE_API csTransform
{
public:
  csTransform(csSpatialState *state,
              const csMatrix4f &matrix,
              const csMatrix4f &parentMatrix = csMatrix4f()
             );

  csTransform &SetTranslation(const csVector3f &translation);
  csTransform &SetTranslation(float x, float y, float z);
  const csVector3f &GetTranslation() const;

  csTransform& SetRotation(const csQuaternion &rotation);
  const csQuaternion &GetRotation() const;

  csTransform& SetScale(const csVector3f &scale);
  const csVector3f &GetScalar() const;

  csTransform& LookAt(const csVector3f &at, const csVector3f &up = csVector3f(0.0f, 1.0f, 0.0f));

  const csMatrix4f &GetMatrix() const;
  const csMatrix4f &GetGlobalMatrix() const;

  csVector3f GetForward() const;
  csVector3f GetBackward() const;

  csVector3f GetRight() const;
  csVector3f GetLeft() const;

  csVector3f GetUp() const;
  csVector3f GetDown() const;



  void Finish ();

private:
  void ExtractTRS();
  void UpdateMatrix() const;

  csSpatialState *m_state;
  csVector3f     m_translation;
  csQuaternion   m_rotation;
  csVector3f     m_scale;

  csMatrix4f         m_parentMatrix;
  mutable bool       m_dirty;
  mutable csMatrix4f m_matrix;
  mutable csMatrix4f m_globalMatrix;
};

}