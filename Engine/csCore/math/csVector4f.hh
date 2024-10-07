#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include <csCore/math/csVector2f.hh>
#include <csCore/math/csVector3f.hh>


namespace cs
{


struct csVector3f;

struct CS_CORE_API csVector4f
{
public:
  float x;
  float y;
  float z;
  float w;

public:
  CS_FORCEINLINE csVector4f()
      : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(1.0f)
  {

  }

  CS_FORCEINLINE csVector4f(const csVector4f &other) = default;

  CS_FORCEINLINE explicit csVector4f(const float *f)
      : x(f[0])
        , y(f[1])
        , z(f[2])
        , w(f[3])
  {
  }

  CS_FORCEINLINE explicit csVector4f(const csVector2f &v2, float z = 0.0f, float w = 1.0f)
      : x(v2.x)
        , y(v2.y)
        , z(z)
        , w(w)
  {

  }

  CS_FORCEINLINE explicit csVector4f(const csVector3f &v3, float w = 1.0f)
      : x(v3.x)
        , y(v3.y)
        , z(v3.z)
        , w(w)
  {

  }

  CS_FORCEINLINE csVector4f(float x, float y, float z, float w)
      : x(x)
        , y(y)
        , z(z)
        , w(w)
  {
  }

  CS_FORCEINLINE csVector4f &operator=(const csVector4f &v) = default;

  CS_FORCEINLINE csVector4f &operator+=(const csVector4f &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    w += v;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator-=(const csVector4f &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return *this;
  }


  CS_FORCEINLINE csVector4f &operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
  }


  CS_FORCEINLINE csVector4f &operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
  }

  CS_FORCEINLINE csVector4f operator-() const
  {
    return csVector4f(-x, -y, -z, -w);
  }

  CS_FORCEINLINE csVector4f operator+() const
  {
    return csVector4f(x, y, z, w);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator+(const csVector4f &v0, const csVector4f &v1)
  {
    return csVector4f(v0.x + v1.x,
                    v0.y + v1.y,
                    v0.z + v1.z,
                    v0.w + v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator+(const csVector4f &v0, float v1)
  {
    return csVector4f(v0.x + v1,
                    v0.y + v1,
                    v0.z + v1,
                    v0.w + v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator+(float v0, const csVector4f &v1)
  {
    return csVector4f(v0 + v1.x,
                    v0 + v1.y,
                    v0 + v1.z,
                    v0 + v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator-(const csVector4f &v0, const csVector4f &v1)
  {
    return csVector4f(v0.x - v1.x,
                    v0.y - v1.y,
                    v0.z - v1.z,
                    v0.w - v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator-(const csVector4f &v0, float v1)
  {
    return csVector4f(v0.x - v1,
                    v0.y - v1,
                    v0.z - v1,
                    v0.w - v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator-(float v0, const csVector4f &v1)
  {
    return csVector4f(v0 - v1.x,
                    v0 - v1.y,
                    v0 - v1.z,
                    v0 - v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend float operator*(const csVector4f &v0, const csVector4f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator*(const csVector4f &v0, float v1)
  {
    return csVector4f(v0.x * v1,
                    v0.y * v1,
                    v0.z * v1,
                    v0.w * v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator*(float v0, const csVector4f &v1)
  {
    return csVector4f(v0 * v1.x,
                    v0 * v1.y,
                    v0 * v1.z,
                    v0 * v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator/(const csVector4f &v0, float v1)
  {
    return csVector4f(v0.x / v1,
                    v0.y / v1,
                    v0.z / v1,
                    v0.w / v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4f operator/(float v0, const csVector4f &v1)
  {
    return csVector4f(v0 / v1.x,
                    v0 / v1.y,
                    v0 / v1.z,
                    v0 / v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE static csVector4f Scale(const csVector4f &v0, const csVector4f &v1)
  {
    return csVector4f(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE float Dot() const
  {
    return x * x + y * y + z * z + w * w;
  }

  CS_NODISCARD CS_FORCEINLINE float Dot(const csVector4f &v) const
  {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  CS_NODISCARD CS_FORCEINLINE static float Dot(const csVector4f &v0, const csVector4f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

  CS_NODISCARD CS_FORCEINLINE float Length() const
  {
    return std::sqrt(x * x + y * y + z * z + w * w);
  }

  CS_FORCEINLINE csVector4f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
  }

  CS_NODISCARD CS_FORCEINLINE csVector4f Normalized() const
  {
    float l = Length();
    return csVector4f(x / l, y / l, z / l, w / l);
  }

  explicit operator csVector3f() const
  {
    return csVector3f(x, y, z);
  }

  explicit operator csVector2f() const
  {
    return csVector2f(x, y);
  }
};


}
