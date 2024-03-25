
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <ceCore/math/math.hh>
#include <ceCore/math/matrix.hh>

namespace ce
{

struct CE_CORE_API Quaternion
{
  float x;
  float y;
  float z;
  float w;

  CE_FORCEINLINE explicit Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
      : x(x)
      , y(y)
      , z(z)
      , w(w)
  {

  }

  CE_FORCEINLINE explicit Quaternion(const Vector3f &v, float w = 0.0f)
      : x(v.x)
      , y(v.y)
      , z(v.z)
      , w(w)
  {

  }

  CE_FORCEINLINE Quaternion &operator=(const Quaternion &other) = default;

  CE_FORCEINLINE void Invert()
  {
    x = -x;
    y = -y;
    z = -z;
  }

  CE_NODISCARD CE_FORCEINLINE Quaternion Inverted() const
  {
    return Quaternion(-x, -y, -z, w);
  }


  CE_FORCEINLINE void Normalize()
  {
    float l = sqrtf(x * x + y * y + z * z + w * w);
    x /= l;
    y /= l;
    z /= l;
    w /= l;
  }

  CE_FORCEINLINE void SetAxisAngle(float axisX, float axisY, float axisZ, float angle)
  {
    float angle2 = angle / 2.0f;
    float c      = std::cos(angle2);
    float s      = std::sin(angle2);
    this->x = axisX * s;
    this->y = axisY * s;
    this->z = axisZ * s;
    this->w = c;
  }


  CE_FORCEINLINE void SetAxisAngle(const Vector3f &v, float angle)
  {
    SetAxisAngle(v.x, v.y, v.z, angle);
  }

  CE_NODISCARD CE_FORCEINLINE static Quaternion FromAxisAngle(const Vector3f &v, float angle)
  {
    return FromAxisAngle(v.x, v.y, v.z, angle);
  }

  CE_NODISCARD CE_FORCEINLINE static Quaternion FromAxisAngle(float x, float y, float z, float angle)
  {
    float angle2 = angle / 2.0f;
    float c      = ceCos(angle2);
    float s      = ceSin(angle2);
    return Quaternion(x * s, y * s, z * s, c);
  }


  CE_NODISCARD CE_FORCEINLINE static Quaternion FromMatrix(const Matrix3f &m)
  {
    float qw  = ceSqrt(1.0f + m.m00 + m.m11 + m.m22) / 2.0f;
    float qw4 = qw * 4.0f;
    return Quaternion(
        (m.m21 - m.m12) / qw4,
        (m.m02 - m.m20) / qw4,
        (m.m10 - m.m01) / qw4,
        qw
    );
  }


  CE_NODISCARD CE_FORCEINLINE static Quaternion FromMatrix(const Matrix4f &m)
  {
    float qw  = ceSqrt(1.0f + m.m00 + m.m11 + m.m22) / 2.0f;
    float qw4 = qw * 4.0f;
    return Quaternion(
        (m.m21 - m.m12) / qw4,
        (m.m02 - m.m20) / qw4,
        (m.m10 - m.m01) / qw4,
        qw
    );
  }


  CE_NODISCARD CE_FORCEINLINE Matrix3f ToMatrix3() const
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

  CE_NODISCARD CE_FORCEINLINE Matrix4f ToMatrix4() const
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

  CE_FORCEINLINE Matrix4f &ToMatrix4(Matrix4f &m) const
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


  CE_FORCEINLINE static Quaternion Blend (const Quaternion &q0, const Quaternion &q1, float f)
  {
    float f0 = 1.0f - f;
    float f1 = f;
    return Quaternion(
        q0.x * f0 + q1.x * f1,
        q0.y * f0 + q1.y * f1,
        q0.z * f0 + q1.z * f1,
        q0.w * f0 + q1.w * f1
        );
  }

  CE_FORCEINLINE friend Quaternion operator*(const Quaternion &q, float f)
  {
    return Quaternion(q.x * f, q.y * f, q.z * f, q.w * f);
  }

  CE_FORCEINLINE friend Quaternion operator+(const Quaternion &q0, const Quaternion &q1)
  {
    return Quaternion(q0.x + q1.x,
                      q0.y + q1.y,
                      q0.z + q1.z,
                      q0.w + q1.w);
  }


  CE_FORCEINLINE friend Quaternion operator-(const Quaternion &q0, const Quaternion &q1)
  {
    return Quaternion(q0.x - q1.x,
                      q0.y - q1.y,
                      q0.z - q1.z,
                      q0.w - q1.w);
  }



  CE_FORCEINLINE friend Quaternion operator*(const Quaternion &q0, const Quaternion &q1)
  {
    return Quaternion(
        q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y,
        q0.w * q1.y - q0.x * q1.z + q0.y * q1.w + q0.z * q1.x,
        q0.w * q1.z + q0.x * q1.y - q0.y * q1.x + q0.z * q1.w,
        q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z
    );
  }

  CE_NODISCARD CE_FORCEINLINE Vector3f Rotate(const Vector3f &v) const
  {
    Quaternion pn = *this * (Quaternion(v) * Inverted());
    return Vector3f(pn.x, pn.y, pn.z);
  }


  CE_FORCEINLINE void Debug() const
  {
    printf("%.2f %.2f %.2f : %.2f", x, y, z, w);
  }
};
}