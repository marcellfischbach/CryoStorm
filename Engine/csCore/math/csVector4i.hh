#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>



namespace cs
{


struct csVector4i
{
public:
  int32_t x;
  int32_t y;
  int32_t z;
  int32_t w;

public:
  CS_FORCEINLINE csVector4i()
      : x(0)
      , y(0)
      , z(0)
      , w(1)
  {

  }

  CS_FORCEINLINE csVector4i(const csVector4i &other) = default;

  CS_FORCEINLINE explicit csVector4i(const int32_t *f)
      : x(f[0])
      , y(f[1])
      , z(f[2])
      , w(f[3])
  {
  }

  CS_FORCEINLINE csVector4i(int32_t x, int32_t y, int32_t z, int32_t w)
      : x(x)
      , y(y)
      , z(z)
      , w(w)
  {
  }

  CS_FORCEINLINE csVector4i &operator=(const csVector4i &v) = default;

  CS_FORCEINLINE csVector4i &operator+=(const csVector4i &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  CS_FORCEINLINE csVector4i &operator+=(int32_t v)
  {
    x += v;
    y += v;
    z += v;
    w += v;
    return *this;
  }

  CS_FORCEINLINE csVector4i &operator-=(const csVector4i &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  CS_FORCEINLINE csVector4i &operator-=(int32_t v)
  {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return *this;
  }


  CS_FORCEINLINE csVector4i &operator*=(int32_t v)
  {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
  }


  CS_FORCEINLINE csVector4i &operator/=(int32_t v)
  {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
  }

  CS_FORCEINLINE csVector4i operator-() const
  {
    return csVector4i(-x, -y, -z, -w);
  }

  CS_FORCEINLINE csVector4i operator+() const
  {
    return csVector4i(x, y, z, w);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator+(const csVector4i &v0, const csVector4i &v1)
  {
    return csVector4i(v0.x + v1.x,
                    v0.y + v1.y,
                    v0.z + v1.z,
                    v0.w + v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator+(const csVector4i &v0, int32_t v1)
  {
    return csVector4i(v0.x + v1,
                    v0.y + v1,
                    v0.z + v1,
                    v0.w + v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator+(int32_t v0, const csVector4i &v1)
  {
    return csVector4i(v0 + v1.x,
                    v0 + v1.y,
                    v0 + v1.z,
                    v0 + v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator-(const csVector4i &v0, const csVector4i &v1)
  {
    return csVector4i(v0.x - v1.x,
                    v0.y - v1.y,
                    v0.z - v1.z,
                    v0.w - v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator-(const csVector4i &v0, int32_t v1)
  {
    return csVector4i(v0.x - v1,
                    v0.y - v1,
                    v0.z - v1,
                    v0.w - v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator-(int32_t v0, const csVector4i &v1)
  {
    return csVector4i(v0 - v1.x,
                    v0 - v1.y,
                    v0 - v1.z,
                    v0 - v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE friend int32_t operator*(const csVector4i &v0, const csVector4i &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator*(const csVector4i &v0, int32_t v1)
  {
    return csVector4i(v0.x * v1,
                    v0.y * v1,
                    v0.z * v1,
                    v0.w * v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator*(int32_t v0, const csVector4i &v1)
  {
    return csVector4i(v0 * v1.x,
                    v0 * v1.y,
                    v0 * v1.z,
                    v0 * v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator/(const csVector4i &v0, int32_t v1)
  {
    return csVector4i(v0.x / v1,
                    v0.y / v1,
                    v0.z / v1,
                    v0.w / v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector4i operator/(int32_t v0, const csVector4i &v1)
  {
    return csVector4i(v0 / v1.x,
                    v0 / v1.y,
                    v0 / v1.z,
                    v0 / v1.w);
  }

  CS_NODISCARD CS_FORCEINLINE static csVector4i Scale(const csVector4i &v0, const csVector4i &v1)
  {
    return csVector4i(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w);
  }


  CS_NODISCARD CS_FORCEINLINE int32_t Dot() const
  {
    return x * x + y * y + z * z + w * w;
  }

  CS_NODISCARD CS_FORCEINLINE int32_t Dot(const csVector4i &v) const
  {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  CS_NODISCARD CS_FORCEINLINE static int32_t Dot(const csVector4i &v0, const csVector4i &v1)
  {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

};


}
