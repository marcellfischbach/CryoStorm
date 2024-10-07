#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include "csMath.hh"
#include <cmath>


namespace cs
{


struct CS_CORE_API csVector2f
{
public:
  float x;
  float y;

public:
  CS_FORCEINLINE csVector2f()
      : x(0.0f)
        , y(0.0f)
  {

  }

  CS_FORCEINLINE explicit csVector2f(const float *f)
      : x(f[0])
        , y(f[1])
  {

  }

  CS_FORCEINLINE csVector2f(const csVector2f &o) = default;

  CS_FORCEINLINE csVector2f(float x, float y)
      : x(x)
        , y(y)
  {

  }

  CS_FORCEINLINE csVector2f &operator=(const csVector2f &v) = default;

  CS_FORCEINLINE csVector2f &operator+=(const csVector2f &v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator+=(float v)
  {
    x += v;
    y += v;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator-=(const csVector2f &v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  CS_FORCEINLINE csVector2f &operator-=(float v)
  {
    x -= v;
    y -= v;
    return *this;
  }


  CS_FORCEINLINE csVector2f &operator*=(float v)
  {
    x *= v;
    y *= v;
    return *this;
  }


  CS_FORCEINLINE csVector2f &operator/=(float v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  CS_FORCEINLINE csVector2f operator-() const
  {
    return csVector2f(-x, -y);
  }

  CS_FORCEINLINE csVector2f operator+() const
  {
    return csVector2f(x, y);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator+(const csVector2f &v0, const csVector2f &v1)
  {
    return csVector2f(v0.x + v1.x,
                    v0.y + v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator+(const csVector2f &v0, float v1)
  {
    return csVector2f(v0.x + v1,
                    v0.y + v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator+(float v0, const csVector2f &v1)
  {
    return csVector2f(v0 + v1.x,
                    v0 + v1.y);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator-(const csVector2f &v0, const csVector2f &v1)
  {
    return csVector2f(v0.x - v1.x,
                    v0.y - v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator-(const csVector2f &v0, float v1)
  {
    return csVector2f(v0.x - v1,
                    v0.y - v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator-(float v0, const csVector2f &v1)
  {
    return csVector2f(v0 - v1.x,
                    v0 - v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE friend float operator*(const csVector2f &v0, const csVector2f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y;
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator*(const csVector2f &v0, float v1)
  {
    return csVector2f(v0.x * v1,
                    v0.y * v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator*(float v0, const csVector2f &v1)
  {
    return csVector2f(v0 * v1.x,
                    v0 * v1.y);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator/(const csVector2f &v0, float v1)
  {
    return csVector2f(v0.x / v1,
                    v0.y / v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2f operator/(float v0, const csVector2f &v1)
  {
    return csVector2f(v0 / v1.x,
                    v0 / v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE static csVector2f Scale(const csVector2f &v0, const csVector2f &v1)
  {
    return csVector2f(v0.x * v1.x, v0.y * v1.y);
  }


  CS_NODISCARD CS_FORCEINLINE float Dot() const
  {
    return x * x + y * y;
  }

  CS_NODISCARD CS_FORCEINLINE float Dot(const csVector2f &v) const
  {
    return x * v.x + y * v.y;
  }

  CS_NODISCARD CS_FORCEINLINE static float Dot(const csVector2f &v0, const csVector2f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y;
  }

  CS_NODISCARD CS_FORCEINLINE float Length() const
  {
    return std::sqrt(x * x + y * y);
  }

  CS_FORCEINLINE csVector2f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    return *this;
  }

  CS_NODISCARD CS_FORCEINLINE csVector2f Normalized() const
  {
    float l = Length();
    return csVector2f(x / l, y / l);
  }

};


}

