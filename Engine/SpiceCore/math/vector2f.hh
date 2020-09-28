#pragma once

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/defs.hh>
#include <SpiceCore/math/math.hh>
#include <stdio.h>



namespace Spice {



struct SPICE_CORE_API Vector2f
{
public:
  float x;
  float y;

public:
  SPICE_FORCEINLINE Vector2f(const float* f)
    : x(f[0])
    , y(f[1])
  {

  }

  SPICE_FORCEINLINE Vector2f(float x = 0.0f, float y = 0.0f)
    : x(x)
    , y(y)
  {

  }

  SPICE_FORCEINLINE void Set(float x, float y)
  {
    this->x = x;
    this->y = y;
  }

  SPICE_FORCEINLINE Vector2f& operator+=(const Vector2f& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  SPICE_FORCEINLINE Vector2f& operator+=(float v)
  {
    x += v;
    y += v;
    return *this;
  }

  SPICE_FORCEINLINE Vector2f& operator-=(const Vector2f& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  SPICE_FORCEINLINE Vector2f& operator-=(float v)
  {
    x -= v;
    y -= v;
    return *this;
  }

  SPICE_FORCEINLINE Vector2f& operator*=(const Vector2f& v)
  {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  SPICE_FORCEINLINE Vector2f& operator*=(float v)
  {
    x *= v;
    y *= v;
    return *this;
  }

  SPICE_FORCEINLINE Vector2f& operator/=(const Vector2f& v)
  {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  SPICE_FORCEINLINE Vector2f& operator/=(float v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  SPICE_FORCEINLINE static Vector2f& Add(const Vector2f& v0, const Vector2f& v1, Vector2f& r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    return r;
  }


  SPICE_FORCEINLINE static Vector2f& Add(const Vector2f& v0, float v1, Vector2f& r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    return r;
  }


  SPICE_FORCEINLINE static Vector2f& Sub(const Vector2f& v0, const Vector2f& v1, Vector2f& r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    return r;
  }


  SPICE_FORCEINLINE static Vector2f& Sub(const Vector2f& v0, float v1, Vector2f& r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    return r;
  }

  SPICE_FORCEINLINE static Vector2f& Mul(const Vector2f& v0, const Vector2f& v1, Vector2f& r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    return r;
  }

  SPICE_FORCEINLINE static Vector2f& Mul(const Vector2f& v0, float v1, Vector2f& r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    return r;
  }

  SPICE_FORCEINLINE static Vector2f& Div(const Vector2f& v0, const Vector2f& v1, Vector2f& r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    return r;
  }

  SPICE_FORCEINLINE static Vector2f& Div(const Vector2f& v0, float v1, Vector2f& r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    return r;
  }

  SPICE_FORCEINLINE float Dot() const
  {
    return x * x + y * y;
  }

  SPICE_FORCEINLINE float Dot(const Vector2f& v) const
  {
    return x * v.x + y * v.y;
  }

  SPICE_FORCEINLINE static float Dot(const Vector2f& v0, const Vector2f& v1)
  {
    return v0.x* v1.x + v0.y * v1.y;
  }

  SPICE_FORCEINLINE float Length() const
  {
    return cesqrt(x * x + y * y);
  }

  SPICE_FORCEINLINE Vector2f& Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    return *this;
  }

  SPICE_FORCEINLINE Vector2f& Normalized(Vector2f & r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    return r;
  }

};



}

