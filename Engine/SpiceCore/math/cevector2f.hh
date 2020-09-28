#pragma once

#include <cecore/ceexport.hh>
#include <cecore/cedefs.hh>
#include <cecore/math/cemath.hh>
#include <stdio.h>



struct CEC_API ceVector2f
{
public:
  float x;
  float y;

public:
  CE_FORCEINLINE ceVector2f(const float* f)
    : x(f[0])
    , y(f[1])
  {

  }

  CE_FORCEINLINE ceVector2f(float x = 0.0f, float y = 0.0f)
    : x(x)
    , y(y)
  {

  }

  CE_FORCEINLINE void Set(float x, float y)
  {
    this->x = x;
    this->y = y;
  }

  CE_FORCEINLINE ceVector2f& operator+=(const ceVector2f& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  CE_FORCEINLINE ceVector2f& operator+=(float v)
  {
    x += v;
    y += v;
    return *this;
  }

  CE_FORCEINLINE ceVector2f& operator-=(const ceVector2f& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  CE_FORCEINLINE ceVector2f& operator-=(float v)
  {
    x -= v;
    y -= v;
    return *this;
  }

  CE_FORCEINLINE ceVector2f& operator*=(const ceVector2f& v)
  {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  CE_FORCEINLINE ceVector2f& operator*=(float v)
  {
    x *= v;
    y *= v;
    return *this;
  }

  CE_FORCEINLINE ceVector2f& operator/=(const ceVector2f& v)
  {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  CE_FORCEINLINE ceVector2f& operator/=(float v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  CE_FORCEINLINE static ceVector2f& Add(const ceVector2f& v0, const ceVector2f& v1, ceVector2f& r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    return r;
  }


  CE_FORCEINLINE static ceVector2f& Add(const ceVector2f& v0, float v1, ceVector2f& r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    return r;
  }


  CE_FORCEINLINE static ceVector2f& Sub(const ceVector2f& v0, const ceVector2f& v1, ceVector2f& r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    return r;
  }


  CE_FORCEINLINE static ceVector2f& Sub(const ceVector2f& v0, float v1, ceVector2f& r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    return r;
  }

  CE_FORCEINLINE static ceVector2f& Mul(const ceVector2f& v0, const ceVector2f& v1, ceVector2f& r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    return r;
  }

  CE_FORCEINLINE static ceVector2f& Mul(const ceVector2f& v0, float v1, ceVector2f& r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    return r;
  }

  CE_FORCEINLINE static ceVector2f& Div(const ceVector2f& v0, const ceVector2f& v1, ceVector2f& r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    return r;
  }

  CE_FORCEINLINE static ceVector2f& Div(const ceVector2f& v0, float v1, ceVector2f& r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    return r;
  }

  CE_FORCEINLINE float Dot() const
  {
    return x * x + y * y;
  }

  CE_FORCEINLINE float Dot(const ceVector2f& v) const
  {
    return x * v.x + y * v.y;
  }

  CE_FORCEINLINE static float Dot(const ceVector2f& v0, const ceVector2f& v1)
  {
    return v0.x* v1.x + v0.y * v1.y;
  }

  CE_FORCEINLINE float Length() const
  {
    return cesqrt(x * x + y * y);
  }

  CE_FORCEINLINE ceVector2f& Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    return *this;
  }

  CE_FORCEINLINE ceVector2f& Normalized(ceVector2f & r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    return r;
  }

};


