#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/vector2f.hh>
#include <cmath>
#include <cstdio>


namespace spc
{


struct SPC_CORE_API Vector3f
{
public:
  float x;
  float y;
  float z;

public:
  SPC_FORCEINLINE Vector3f()
      : x(0.0f)
        , y(0.0f)
        , z(0.0f)
  {
  }

  SPC_FORCEINLINE Vector3f(const Vector3f &other) = default;


  SPC_FORCEINLINE explicit Vector3f(const Vector2f &v2, float z = 0.0f)
      : x(v2.x)
        , y(v2.y)
        , z(z)
  {

  }


  SPC_FORCEINLINE explicit Vector3f(const float *f)
      : x(f[0])
        , y(f[1])
        , z(f[2])
  {
  }

  SPC_FORCEINLINE Vector3f(float x, float y, float z)
      : x(x)
        , y(y)
        , z(z)
  {

  }

  SPC_FORCEINLINE Vector3f &operator=(const Vector3f &v) = default;

  SPC_FORCEINLINE Vector3f &operator+=(const Vector3f &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  SPC_FORCEINLINE Vector3f &operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    return *this;
  }

  SPC_FORCEINLINE Vector3f &operator-=(const Vector3f &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  SPC_FORCEINLINE Vector3f &operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    return *this;
  }


  SPC_FORCEINLINE Vector3f &operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    return *this;
  }


  SPC_FORCEINLINE Vector3f &operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    return *this;
  }

  SPC_FORCEINLINE Vector3f operator-() const
  {
    return Vector3f(-x, -y, -z);
  }

  SPC_FORCEINLINE Vector3f operator+() const
  {
    return Vector3f(x, y, z);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator+(const Vector3f &v0, const Vector3f &v1)
  {
    return Vector3f(v0.x + v1.x,
                    v0.y + v1.y,
                    v0.z + v1.z);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator+(const Vector3f &v0, float v1)
  {
    return Vector3f(v0.x + v1,
                    v0.y + v1,
                    v0.z + v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator+(float v0, const Vector3f &v1)
  {
    return Vector3f(v0 + v1.x,
                    v0 + v1.y,
                    v0 + v1.z);
  }


  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator-(const Vector3f &v0, const Vector3f &v1)
  {
    return Vector3f(v0.x - v1.x,
                    v0.y - v1.y,
                    v0.z - v1.z);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator-(const Vector3f &v0, float v1)
  {
    return Vector3f(v0.x - v1,
                    v0.y - v1,
                    v0.z - v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator-(float v0, const Vector3f &v1)
  {
    return Vector3f(v0 - v1.x,
                    v0 - v1.y,
                    v0 - v1.z);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend float operator*(const Vector3f &v0, const Vector3f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator%(const Vector3f &v0, const Vector3f &v1)
  {
    return Vector3f(
        v0.y * v1.z - v0.z * v1.y,
        v0.z * v1.x - v0.x * v1.z,
        v0.x * v1.y - v0.y * v1.x);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator*(const Vector3f &v0, float v1)
  {
    return Vector3f(v0.x * v1,
                    v0.y * v1,
                    v0.z * v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator*(float v0, const Vector3f &v1)
  {
    return Vector3f(v0 * v1.x,
                    v0 * v1.y,
                    v0 * v1.z);
  }


  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator/(const Vector3f &v0, float v1)
  {
    return Vector3f(v0.x / v1,
                    v0.y / v1,
                    v0.z / v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector3f operator/(float v0, const Vector3f &v1)
  {
    return Vector3f(v0 / v1.x,
                    v0 / v1.y,
                    v0 / v1.z);
  }

  SPC_NODISCARD SPC_FORCEINLINE static Vector3f Scale(const Vector3f &v0, const Vector3f &v1)
  {
    return Vector3f(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z);
  }


  SPC_NODISCARD SPC_FORCEINLINE float Dot() const
  {
    return x * x + y * y + z * z;
  }

  SPC_NODISCARD SPC_FORCEINLINE float Dot(const Vector3f &v) const
  {
    return x * v.x + y * v.y + z * v.z;
  }

  SPC_NODISCARD SPC_FORCEINLINE static float Dot(const Vector3f &v0, const Vector3f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
  }

  SPC_NODISCARD SPC_FORCEINLINE float Length() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  SPC_FORCEINLINE Vector3f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    return *this;
  }

  SPC_NODISCARD SPC_FORCEINLINE Vector3f Normalized() const
  {
    float l = Length();
    return Vector3f(x / l, y / l, z / l);
  }

  explicit operator Vector2f() const
  {
    return Vector2f(x, y);
  }
};


}

