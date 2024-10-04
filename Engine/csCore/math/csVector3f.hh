#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include <csCore/math/csVector2f.hh>
#include <cmath>
#include <cstdio>


namespace cryo
{


struct CS_CORE_API csVector3f
{
public:
  float x;
  float y;
  float z;

public:
  CS_FORCEINLINE csVector3f()
      : x(0.0f)
        , y(0.0f)
        , z(0.0f)
  {
  }

  CS_FORCEINLINE csVector3f(const csVector3f &other) = default;


  CS_FORCEINLINE explicit csVector3f(const csVector2f &v2, float z = 0.0f)
      : x(v2.x)
        , y(v2.y)
        , z(z)
  {

  }


  CS_FORCEINLINE explicit csVector3f(const float *f)
      : x(f[0])
        , y(f[1])
        , z(f[2])
  {
  }

  CS_FORCEINLINE csVector3f(float x, float y, float z)
      : x(x)
        , y(y)
        , z(z)
  {

  }

  CS_FORCEINLINE csVector3f &operator=(const csVector3f &v) = default;

  CS_FORCEINLINE csVector3f &operator+=(const csVector3f &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator-=(const csVector3f &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    return *this;
  }


  CS_FORCEINLINE csVector3f &operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    return *this;
  }


  CS_FORCEINLINE csVector3f &operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    return *this;
  }

  CS_FORCEINLINE csVector3f operator-() const
  {
    return csVector3f(-x, -y, -z);
  }

  CS_FORCEINLINE csVector3f operator+() const
  {
    return csVector3f(x, y, z);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator+(const csVector3f &v0, const csVector3f &v1)
  {
    return csVector3f(v0.x + v1.x,
                    v0.y + v1.y,
                    v0.z + v1.z);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator+(const csVector3f &v0, float v1)
  {
    return csVector3f(v0.x + v1,
                    v0.y + v1,
                    v0.z + v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator+(float v0, const csVector3f &v1)
  {
    return csVector3f(v0 + v1.x,
                    v0 + v1.y,
                    v0 + v1.z);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator-(const csVector3f &v0, const csVector3f &v1)
  {
    return csVector3f(v0.x - v1.x,
                    v0.y - v1.y,
                    v0.z - v1.z);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator-(const csVector3f &v0, float v1)
  {
    return csVector3f(v0.x - v1,
                    v0.y - v1,
                    v0.z - v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator-(float v0, const csVector3f &v1)
  {
    return csVector3f(v0 - v1.x,
                    v0 - v1.y,
                    v0 - v1.z);
  }

  CS_NODISCARD CS_FORCEINLINE friend float operator*(const csVector3f &v0, const csVector3f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator%(const csVector3f &v0, const csVector3f &v1)
  {
    return csVector3f(
        v0.y * v1.z - v0.z * v1.y,
        v0.z * v1.x - v0.x * v1.z,
        v0.x * v1.y - v0.y * v1.x);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator*(const csVector3f &v0, float v1)
  {
    return csVector3f(v0.x * v1,
                    v0.y * v1,
                    v0.z * v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator*(float v0, const csVector3f &v1)
  {
    return csVector3f(v0 * v1.x,
                    v0 * v1.y,
                    v0 * v1.z);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator/(const csVector3f &v0, float v1)
  {
    return csVector3f(v0.x / v1,
                    v0.y / v1,
                    v0.z / v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector3f operator/(float v0, const csVector3f &v1)
  {
    return csVector3f(v0 / v1.x,
                    v0 / v1.y,
                    v0 / v1.z);
  }

  CS_NODISCARD CS_FORCEINLINE static csVector3f Scale(const csVector3f &v0, const csVector3f &v1)
  {
    return csVector3f(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z);
  }


  CS_NODISCARD CS_FORCEINLINE float Dot() const
  {
    return x * x + y * y + z * z;
  }

  CS_NODISCARD CS_FORCEINLINE float Dot(const csVector3f &v) const
  {
    return x * v.x + y * v.y + z * v.z;
  }

  CS_NODISCARD CS_FORCEINLINE static float Dot(const csVector3f &v0, const csVector3f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
  }

  CS_NODISCARD CS_FORCEINLINE float Length() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  CS_FORCEINLINE csVector3f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    return *this;
  }

  CS_NODISCARD CS_FORCEINLINE csVector3f Normalized() const
  {
    float l = Length();
    return csVector3f(x / l, y / l, z / l);
  }

  explicit operator csVector2f() const
  {
    return csVector2f(x, y);
  }
};


}

