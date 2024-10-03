#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>



namespace ce
{


struct Vector4i
{
public:
  int x;
  int y;
  int z;
  int w;

public:
  CS_FORCEINLINE Vector4i()
      : x(0)
      , y(0)
      , z(0)
      , w(1)
  {

  }

  CS_FORCEINLINE Vector4i(const Vector4i &other) = default;

  CS_FORCEINLINE explicit Vector4i(const int *f)
      : x(f[0])
      , y(f[1])
      , z(f[2])
      , w(f[3])
  {
  }

  CS_FORCEINLINE Vector4i(int x, int y, int z, int w)
      : x(x)
      , y(y)
      , z(z)
      , w(w)
  {
  }

  CS_FORCEINLINE Vector4i &operator=(const Vector4i &v) = default;

  CS_FORCEINLINE Vector4i &operator+=(const Vector4i &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  CS_FORCEINLINE Vector4i &operator+=(int v)
  {
    x += v;
    y += v;
    z += v;
    w += v;
    return *this;
  }

  CS_FORCEINLINE Vector4i &operator-=(const Vector4i &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  CS_FORCEINLINE Vector4i &operator-=(int v)
  {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return *this;
  }


  CS_FORCEINLINE Vector4i &operator*=(int v)
  {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
  }


  CS_FORCEINLINE Vector4i &operator/=(int v)
  {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
  }

  CS_FORCEINLINE Vector4i operator-() const
  {
    return Vector4i(-x, -y, -z, -w);
  }

  CS_FORCEINLINE Vector4i operator+() const
  {
    return Vector4i(x, y, z, w);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator+(const Vector4i &v0, const Vector4i &v1)
  {
    return Vector4i(v0.x + v1.x,
                    v0.y + v1.y,
                    v0.z + v1.z,
                    v0.w + v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator+(const Vector4i &v0, int v1)
  {
    return Vector4i(v0.x + v1,
                    v0.y + v1,
                    v0.z + v1,
                    v0.w + v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator+(int v0, const Vector4i &v1)
  {
    return Vector4i(v0 + v1.x,
                    v0 + v1.y,
                    v0 + v1.z,
                    v0 + v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator-(const Vector4i &v0, const Vector4i &v1)
  {
    return Vector4i(v0.x - v1.x,
                    v0.y - v1.y,
                    v0.z - v1.z,
                    v0.w - v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator-(const Vector4i &v0, int v1)
  {
    return Vector4i(v0.x - v1,
                    v0.y - v1,
                    v0.z - v1,
                    v0.w - v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator-(int v0, const Vector4i &v1)
  {
    return Vector4i(v0 - v1.x,
                    v0 - v1.y,
                    v0 - v1.z,
                    v0 - v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend int operator*(const Vector4i &v0, const Vector4i &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator*(const Vector4i &v0, int v1)
  {
    return Vector4i(v0.x * v1,
                    v0.y * v1,
                    v0.z * v1,
                    v0.w * v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator*(int v0, const Vector4i &v1)
  {
    return Vector4i(v0 * v1.x,
                    v0 * v1.y,
                    v0 * v1.z,
                    v0 * v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator/(const Vector4i &v0, int v1)
  {
    return Vector4i(v0.x / v1,
                    v0.y / v1,
                    v0.z / v1,
                    v0.w / v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector4i operator/(int v0, const Vector4i &v1)
  {
    return Vector4i(v0 / v1.x,
                    v0 / v1.y,
                    v0 / v1.z,
                    v0 / v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE static Vector4i Scale(const Vector4i &v0, const Vector4i &v1)
  {
    return Vector4i(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE int Dot() const
  {
    return x * x + y * y + z * z + w * w;
  }

  CS_NODISCARD CS_FORCEINLINE int Dot(const Vector4i &v) const
  {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  CS_NODISCARD CS_FORCEINLINE static int Dot(const Vector4i &v0, const Vector4i &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

};


}
