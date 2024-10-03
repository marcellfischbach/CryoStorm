#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <ceCore/math/vector2f.hh>
#include <ceCore/math/vector3f.hh>


namespace ce
{


struct Vector3f;

struct CS_CORE_API Vector4f
{
public:
  float x;
  float y;
  float z;
  float w;

public:
  CS_FORCEINLINE Vector4f()
      : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(1.0f)
  {

  }

  CS_FORCEINLINE Vector4f(const Vector4f &other) = default;

  CS_FORCEINLINE explicit Vector4f(const float *f)
      : x(f[0])
        , y(f[1])
        , z(f[2])
        , w(f[3])
  {
  }

  CS_FORCEINLINE explicit Vector4f(const Vector2f &v2, float z = 0.0f, float w = 1.0f)
      : x(v2.x)
        , y(v2.y)
        , z(z)
        , w(w)
  {

  }

  CS_FORCEINLINE explicit Vector4f(const Vector3f &v3, float w = 1.0f)
      : x(v3.x)
        , y(v3.y)
        , z(v3.z)
        , w(w)
  {

  }

  CS_FORCEINLINE Vector4f(float x, float y, float z, float w)
      : x(x)
        , y(y)
        , z(z)
        , w(w)
  {
  }

  CS_FORCEINLINE Vector4f &operator=(const Vector4f &v) = default;

  CS_FORCEINLINE Vector4f &operator+=(const Vector4f &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  CS_FORCEINLINE Vector4f &operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    w += v;
    return *this;
  }

  CS_FORCEINLINE Vector4f &operator-=(const Vector4f &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  CS_FORCEINLINE Vector4f &operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return *this;
  }


  CS_FORCEINLINE Vector4f &operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
  }


  CS_FORCEINLINE Vector4f &operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
  }

  CS_FORCEINLINE Vector4f operator-() const
  {
    return Vector4f(-x, -y, -z, -w);
  }

  CS_FORCEINLINE Vector4f operator+() const
  {
    return Vector4f(x, y, z, w);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator+(const Vector4f &v0, const Vector4f &v1)
  {
    return Vector4f(v0.x + v1.x,
                    v0.y + v1.y,
                    v0.z + v1.z,
                    v0.w + v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator+(const Vector4f &v0, float v1)
  {
    return Vector4f(v0.x + v1,
                    v0.y + v1,
                    v0.z + v1,
                    v0.w + v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator+(float v0, const Vector4f &v1)
  {
    return Vector4f(v0 + v1.x,
                    v0 + v1.y,
                    v0 + v1.z,
                    v0 + v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator-(const Vector4f &v0, const Vector4f &v1)
  {
    return Vector4f(v0.x - v1.x,
                    v0.y - v1.y,
                    v0.z - v1.z,
                    v0.w - v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator-(const Vector4f &v0, float v1)
  {
    return Vector4f(v0.x - v1,
                    v0.y - v1,
                    v0.z - v1,
                    v0.w - v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator-(float v0, const Vector4f &v1)
  {
    return Vector4f(v0 - v1.x,
                    v0 - v1.y,
                    v0 - v1.z,
                    v0 - v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend float operator*(const Vector4f &v0, const Vector4f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator*(const Vector4f &v0, float v1)
  {
    return Vector4f(v0.x * v1,
                    v0.y * v1,
                    v0.z * v1,
                    v0.w * v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator*(float v0, const Vector4f &v1)
  {
    return Vector4f(v0 * v1.x,
                    v0 * v1.y,
                    v0 * v1.z,
                    v0 * v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator/(const Vector4f &v0, float v1)
  {
    return Vector4f(v0.x / v1,
                    v0.y / v1,
                    v0.z / v1,
                    v0.w / v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4f operator/(float v0, const Vector4f &v1)
  {
    return Vector4f(v0 / v1.x,
                    v0 / v1.y,
                    v0 / v1.z,
                    v0 / v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE static Vector4f Scale(const Vector4f &v0, const Vector4f &v1)
  {
    return Vector4f(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE float Dot() const
  {
    return x * x + y * y + z * z + w * w;
  }

  CS_NODISCARD CS_FORCEINLINE float Dot(const Vector4f &v) const
  {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  CS_NODISCARD CS_FORCEINLINE static float Dot(const Vector4f &v0, const Vector4f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

  CS_NODISCARD CS_FORCEINLINE float Length() const
  {
    return std::sqrt(x * x + y * y + z * z + w * w);
  }

  CS_FORCEINLINE Vector4f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
  }

  CS_NODISCARD CS_FORCEINLINE Vector4f Normalized() const
  {
    float l = Length();
    return Vector4f(x / l, y / l, z / l, w / l);
  }

  explicit operator Vector3f() const
  {
    return Vector3f(x, y, z);
  }

  explicit operator Vector2f() const
  {
    return Vector2f(x, y);
  }
};


}
