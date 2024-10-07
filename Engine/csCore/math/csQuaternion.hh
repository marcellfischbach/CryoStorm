
#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include "csMath.hh"
#include <csCore/math/csMatrix.hh>

namespace cs
{

struct CS_CORE_API csQuaternion
{
  float x;
  float y;
  float z;
  float w;

  CS_FORCEINLINE explicit csQuaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
      : x(x)
      , y(y)
      , z(z)
      , w(w)
  {

  }

  CS_FORCEINLINE explicit csQuaternion(const csVector3f &v, float w = 0.0f)
      : x(v.x)
      , y(v.y)
      , z(v.z)
      , w(w)
  {

  }

  CS_FORCEINLINE csQuaternion &operator=(const csQuaternion &other) = default;

  CS_FORCEINLINE void Invert()
  {
    x = -x;
    y = -y;
    z = -z;
  }

  CS_NODISCARD CS_FORCEINLINE csQuaternion Inverted() const
  {
    return csQuaternion(-x, -y, -z, w);
  }


  CS_FORCEINLINE void Normalize()
  {
    float l = sqrtf(x * x + y * y + z * z + w * w);
    x /= l;
    y /= l;
    z /= l;
    w /= l;
  }

  CS_FORCEINLINE void SetAxisAngle(float axisX, float axisY, float axisZ, float angle)
  {
    float angle2 = angle / 2.0f;
    float c      = std::cos(angle2);
    float s      = std::sin(angle2);
    this->x = axisX * s;
    this->y = axisY * s;
    this->z = axisZ * s;
    this->w = c;
  }


  CS_FORCEINLINE void SetAxisAngle(const csVector3f &v, float angle)
  {
    SetAxisAngle(v.x, v.y, v.z, angle);
  }

  CS_NODISCARD CS_FORCEINLINE static csQuaternion FromAxisAngle(const csVector3f &v, float angle)
  {
    return FromAxisAngle(v.x, v.y, v.z, angle);
  }

  CS_NODISCARD CS_FORCEINLINE static csQuaternion FromAxisAngle(float x, float y, float z, float angle)
  {
    float angle2 = angle / 2.0f;
    float c      = ceCos(angle2);
    float s      = ceSin(angle2);
    return csQuaternion(x * s, y * s, z * s, c);
  }


  CS_NODISCARD CS_FORCEINLINE static csQuaternion FromMatrix(const csMatrix3f &m)
  {
    float qw  = ceSqrt(1.0f + m.m00 + m.m11 + m.m22) / 2.0f;
    float qw4 = qw * 4.0f;
    return csQuaternion(
        (m.m21 - m.m12) / qw4,
        (m.m02 - m.m20) / qw4,
        (m.m10 - m.m01) / qw4,
        qw
    );
  }


  CS_NODISCARD CS_FORCEINLINE static csQuaternion FromMatrix(const csMatrix4f &m)
  {
    float qw  = ceSqrt(1.0f + m.m00 + m.m11 + m.m22) / 2.0f;
    float qw4 = qw * 4.0f;
    return csQuaternion(
        (m.m21 - m.m12) / qw4,
        (m.m02 - m.m20) / qw4,
        (m.m10 - m.m01) / qw4,
        qw
    );
  }


  CS_NODISCARD CS_FORCEINLINE csMatrix3f ToMatrix3() const
  {
    float sqx = x * x;
    float sqy = y * y;
    float sqz = z * z;
    float sqw = w * w;
    return csMatrix3f(
        2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y),
        2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x),
        2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f);

  }

  CS_NODISCARD CS_FORCEINLINE csMatrix4f ToMatrix4() const
  {
    float sqx = x * x;
    float sqy = y * y;
    float sqz = z * z;
    float sqw = w * w;
    return csMatrix4f(
        2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y), 0.0f,
        2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x), 0.0f,
        2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

  }

  CS_FORCEINLINE csMatrix4f &ToMatrix4(csMatrix4f &m) const
  {
    float sqx = x * x;
    float sqy = y * y;
    float sqz = z * z;
    float sqw = w * w;
    m.Set(
        2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y), 0.0f,
        2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x), 0.0f,
        2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    return m;
  }


  CS_FORCEINLINE static csQuaternion Blend(const csQuaternion &q0, const csQuaternion &q1, float f)
  {
    float f0 = 1.0f - f;
    float f1 = f;
    return csQuaternion(
        q0.x * f0 + q1.x * f1,
        q0.y * f0 + q1.y * f1,
        q0.z * f0 + q1.z * f1,
        q0.w * f0 + q1.w * f1
        );
  }

  CS_FORCEINLINE friend csQuaternion operator*(const csQuaternion &q, float f)
  {
    return csQuaternion(q.x * f, q.y * f, q.z * f, q.w * f);
  }

  CS_FORCEINLINE friend csQuaternion operator+(const csQuaternion &q0, const csQuaternion &q1)
  {
    return csQuaternion(q0.x + q1.x,
                      q0.y + q1.y,
                      q0.z + q1.z,
                      q0.w + q1.w);
  }


  CS_FORCEINLINE friend csQuaternion operator-(const csQuaternion &q0, const csQuaternion &q1)
  {
    return csQuaternion(q0.x - q1.x,
                      q0.y - q1.y,
                      q0.z - q1.z,
                      q0.w - q1.w);
  }



  CS_FORCEINLINE friend csQuaternion operator*(const csQuaternion &q0, const csQuaternion &q1)
  {
    return csQuaternion(
        q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y,
        q0.w * q1.y - q0.x * q1.z + q0.y * q1.w + q0.z * q1.x,
        q0.w * q1.z + q0.x * q1.y - q0.y * q1.x + q0.z * q1.w,
        q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z
    );
  }

  CS_NODISCARD CS_FORCEINLINE csVector3f Rotate(const csVector3f &v) const
  {
    csQuaternion pn = *this * (csQuaternion(v) * Inverted());
    return csVector3f(pn.x, pn.y, pn.z);
  }


  CS_FORCEINLINE void Debug() const
  {
    printf("%.2f %.2f %.2f : %.2f", x, y, z, w);
  }
};
}