
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/math.hh>
#include <spcCore/math/matrix.hh>

namespace spc
{

struct SPC_CORE_API Quaternion
{
  float x;
  float y;
  float z;
  float w;

  SPC_FORCEINLINE explicit Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
      : x(x), y(y), z(z), w(w)
  {

  }

  SPC_FORCEINLINE explicit Quaternion(const Vector3f &v, float w = 0.0f)
      : x(v.x), y(v.y), z(v.z), w(w)
  {

  }

  SPC_FORCEINLINE Quaternion &operator=(const Quaternion &other) = default;

  SPC_FORCEINLINE void Invert()
  {
    x = -x;
    y = -y;
    z = -z;
  }

  SPC_NODISCARD SPC_FORCEINLINE Quaternion Inverted() const
  {
    return Quaternion(-x, -y, -z, w);
  }


  SPC_FORCEINLINE void Normalize()
  {
    float l = sqrtf(x * x + y * y + z * z + w * w);
    x /= l;
    y /= l;
    z /= l;
    w /= l;
  }

  SPC_FORCEINLINE void SetAxisAngle(float axisX, float axisY, float axisZ, float angle)
  {
    float angle2 = angle / 2.0f;
    float c = std::cos(angle2);
    float s = std::sin(angle2);
    this->x = axisX * s;
    this->y = axisY * s;
    this->z = axisZ * s;
    this->w = c;
  }

  SPC_FORCEINLINE void SetAxisAngle(const Vector3f &v, float angle)
  {
    SetAxisAngle(v.x, v.y, v.z, angle);
  }

  SPC_NODISCARD SPC_FORCEINLINE static Quaternion FromAxisAngle(const Vector3f &v, float angle)
  {
    return FromAxisAngle(v.x, v.y, v.z, angle);
  }

  SPC_NODISCARD SPC_FORCEINLINE static Quaternion FromAxisAngle(float x, float y, float z, float angle)
  {
    float angle2 = angle / 2.0f;
    float c = spcCos(angle2);
    float s = spcSin(angle2);
    return Quaternion(x * s, y * s, z * s, c);
  }


  SPC_NODISCARD SPC_FORCEINLINE static Quaternion FromMatrix(const Matrix3f &m)
  {
    float qw = spcSqrt(1.0f + m.m00 + m.m11 + m.m22) / 2.0f;
    float qw4 = qw * 4.0f;
    return Quaternion(
        (m.m21 - m.m12) / qw4,
        (m.m02 - m.m20) / qw4,
        (m.m10 - m.m01) / qw4,
        qw
    );
  }


  SPC_NODISCARD SPC_FORCEINLINE Matrix3f ToMatrix3() const
  {
    float sqx = x * x;
    float sqy = y * y;
    float sqz = z * z;
    float sqw = w * w;
    return Matrix3f(
        2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y),
        2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x),
        2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f);

  }

  SPC_NODISCARD SPC_FORCEINLINE Matrix4f ToMatrix4() const
  {
    float sqx = x * x;
    float sqy = y * y;
    float sqz = z * z;
    float sqw = w * w;
    return Matrix4f(
        2.0f * (sqw + sqx) - 1.0f, 2.0f * (x * y - w * z), 2.0f * (x * z + w * y), 0.0f,
        2.0f * (x * y + w * z), 2.0f * (sqw + sqy) - 1.0f, 2.0f * (y * z - w * x), 0.0f,
        2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 2.0f * (sqw + sqz) - 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

  }


  SPC_FORCEINLINE friend Quaternion operator*(const Quaternion &q0, const Quaternion &q1)
  {
    return Quaternion(
        q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y,
        q0.w * q1.y - q0.x * q1.z + q0.y * q1.w + q0.z * q1.x,
        q0.w * q1.z + q0.x * q1.y - q0.y * q1.x + q0.z * q1.w,
        q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z
    );
  }

  SPC_NODISCARD SPC_FORCEINLINE Vector3f Rotate(const Vector3f &v) const
  {
    Quaternion pn = *this * (Quaternion(v) * Inverted());
    return Vector3f(pn.x, pn.y, pn.z);
  }


  SPC_FORCEINLINE void Debug() const
  {
    printf("%.2f %.2f %.2f : %.2f", x, y, z, w);
  }
};
}