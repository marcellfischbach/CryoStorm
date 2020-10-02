#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/math.hh>
#include <stdio.h>



namespace spc {



struct SPC_CORE_API Vector2f
{
public:
  float x;
  float y;

public:
  SPC_FORCEINLINE Vector2f(const float* f)
    : x(f[0])
    , y(f[1])
  {

  }

  SPC_FORCEINLINE Vector2f(float x = 0.0f, float y = 0.0f)
    : x(x)
    , y(y)
  {

  }

  SPC_FORCEINLINE void Set(float x, float y)
  {
    this->x = x;
    this->y = y;
  }

  SPC_FORCEINLINE Vector2f& operator+=(const Vector2f& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2f& operator+=(float v)
  {
    x += v;
    y += v;
    return *this;
  }

  SPC_FORCEINLINE Vector2f& operator-=(const Vector2f& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2f& operator-=(float v)
  {
    x -= v;
    y -= v;
    return *this;
  }

  SPC_FORCEINLINE Vector2f& operator*=(const Vector2f& v)
  {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2f& operator*=(float v)
  {
    x *= v;
    y *= v;
    return *this;
  }

  SPC_FORCEINLINE Vector2f& operator/=(const Vector2f& v)
  {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2f& operator/=(float v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  SPC_FORCEINLINE static Vector2f& Add(const Vector2f& v0, const Vector2f& v1, Vector2f& r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    return r;
  }


  SPC_FORCEINLINE static Vector2f& Add(const Vector2f& v0, float v1, Vector2f& r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    return r;
  }


  SPC_FORCEINLINE static Vector2f& Sub(const Vector2f& v0, const Vector2f& v1, Vector2f& r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    return r;
  }


  SPC_FORCEINLINE static Vector2f& Sub(const Vector2f& v0, float v1, Vector2f& r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    return r;
  }

  SPC_FORCEINLINE static Vector2f& Mul(const Vector2f& v0, const Vector2f& v1, Vector2f& r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    return r;
  }

  SPC_FORCEINLINE static Vector2f& Mul(const Vector2f& v0, float v1, Vector2f& r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    return r;
  }

  SPC_FORCEINLINE static Vector2f& Div(const Vector2f& v0, const Vector2f& v1, Vector2f& r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    return r;
  }

  SPC_FORCEINLINE static Vector2f& Div(const Vector2f& v0, float v1, Vector2f& r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    return r;
  }

  SPC_FORCEINLINE float Dot() const
  {
    return x * x + y * y;
  }

  SPC_FORCEINLINE float Dot(const Vector2f& v) const
  {
    return x * v.x + y * v.y;
  }

  SPC_FORCEINLINE static float Dot(const Vector2f& v0, const Vector2f& v1)
  {
    return v0.x* v1.x + v0.y * v1.y;
  }

  SPC_FORCEINLINE float Length() const
  {
    return spcSqrt(x * x + y * y);
  }

  SPC_FORCEINLINE Vector2f& Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    return *this;
  }

  SPC_FORCEINLINE Vector2f& Normalized(Vector2f & r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    return r;
  }

};



}

