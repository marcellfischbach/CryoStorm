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
  CE_FORCEINLINE Vector4i()
      : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(1.0f)
  {

  }

  CE_FORCEINLINE Vector4i(const Vector4i &other) = default;

  CE_FORCEINLINE explicit Vector4i(const int *f)
      : x(f[0])
        , y(f[1])
        , z(f[2])
        , w(f[3])
  {
  }

  CE_FORCEINLINE Vector4i(int x, int y, int z, int w)
      : x(x)
        , y(y)
        , z(z)
        , w(w)
  {
  }

  CE_FORCEINLINE Vector4i &operator=(const Vector4i &v) = default;

  CE_FORCEINLINE Vector4i &operator+=(const Vector4i &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  CE_FORCEINLINE Vector4i &operator+=(int v)
  {
    x += v;
    y += v;
    z += v;
    w += v;
    return *this;
  }

  CE_FORCEINLINE Vector4i &operator-=(const Vector4i &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  CE_FORCEINLINE Vector4i &operator-=(int v)
  {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return *this;
  }


  CE_FORCEINLINE Vector4i &operator*=(int v)
  {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
  }


  CE_FORCEINLINE Vector4i &operator/=(int v)
  {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
  }

  CE_FORCEINLINE Vector4i operator-() const
  {
    return Vector4i(-x, -y, -z, -w);
  }

  CE_FORCEINLINE Vector4i operator+() const
  {
    return Vector4i(x, y, z, w);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator+(const Vector4i &v0, const Vector4i &v1)
  {
    return Vector4i(v0.x + v1.x,
                    v0.y + v1.y,
                    v0.z + v1.z,
                    v0.w + v1.w);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator+(const Vector4i &v0, int v1)
  {
    return Vector4i(v0.x + v1,
                    v0.y + v1,
                    v0.z + v1,
                    v0.w + v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator+(int v0, const Vector4i &v1)
  {
    return Vector4i(v0 + v1.x,
                    v0 + v1.y,
                    v0 + v1.z,
                    v0 + v1.w);
  }


  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator-(const Vector4i &v0, const Vector4i &v1)
  {
    return Vector4i(v0.x - v1.x,
                    v0.y - v1.y,
                    v0.z - v1.z,
                    v0.w - v1.w);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator-(const Vector4i &v0, int v1)
  {
    return Vector4i(v0.x - v1,
                    v0.y - v1,
                    v0.z - v1,
                    v0.w - v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator-(int v0, const Vector4i &v1)
  {
    return Vector4i(v0 - v1.x,
                    v0 - v1.y,
                    v0 - v1.z,
                    v0 - v1.w);
  }

  CE_NODISCARD CE_FORCEINLINE friend int operator*(const Vector4i &v0, const Vector4i &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator*(const Vector4i &v0, int v1)
  {
    return Vector4i(v0.x * v1,
                    v0.y * v1,
                    v0.z * v1,
                    v0.w * v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator*(int v0, const Vector4i &v1)
  {
    return Vector4i(v0 * v1.x,
                    v0 * v1.y,
                    v0 * v1.z,
                    v0 * v1.w);
  }


  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator/(const Vector4i &v0, int v1)
  {
    return Vector4i(v0.x / v1,
                    v0.y / v1,
                    v0.z / v1,
                    v0.w / v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector4i operator/(int v0, const Vector4i &v1)
  {
    return Vector4i(v0 / v1.x,
                    v0 / v1.y,
                    v0 / v1.z,
                    v0 / v1.w);
  }

  CE_NODISCARD CE_FORCEINLINE static Vector4i Scale(const Vector4i &v0, const Vector4i &v1)
  {
    return Vector4i(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w);
  }


  CE_NODISCARD CE_FORCEINLINE int Dot() const
  {
    return x * x + y * y + z * z + w * w;
  }

  CE_NODISCARD CE_FORCEINLINE int Dot(const Vector4i &v) const
  {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  CE_NODISCARD CE_FORCEINLINE static int Dot(const Vector4i &v0, const Vector4i &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

};


}
