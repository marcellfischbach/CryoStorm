#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/math.hh>
#include <stdio.h>



namespace spc {



struct SPC_CORE_API Vector2i
{
public:
  int x;
  int y;

public:
  SPC_FORCEINLINE Vector2i(const int* f)
    : x(f[0])
    , y(f[1])
  {

  }

  SPC_FORCEINLINE Vector2i(int x = 0.0f, int y = 0.0f)
    : x(x)
    , y(y)
  {

  }

  SPC_FORCEINLINE void Set(int x, int y)
  {
    this->x = x;
    this->y = y;
  }

  SPC_FORCEINLINE Vector2i& operator+=(const Vector2i& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2i& operator+=(int v)
  {
    x += v;
    y += v;
    return *this;
  }

  SPC_FORCEINLINE Vector2i& operator-=(const Vector2i& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2i& operator-=(int v)
  {
    x -= v;
    y -= v;
    return *this;
  }

  SPC_FORCEINLINE Vector2i& operator*=(const Vector2i& v)
  {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2i& operator*=(int v)
  {
    x *= v;
    y *= v;
    return *this;
  }

  SPC_FORCEINLINE Vector2i& operator/=(const Vector2i& v)
  {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2i& operator/=(int v)
  {
    x /= v;
    y /= v;
    return *this;
  }


  SPC_FORCEINLINE Vector2i operator-() const
  {
    return Vector2i(-x, -y);
  }

  SPC_FORCEINLINE Vector2i operator+() const
  {
    return Vector2i(x, y);
  }

  SPC_FORCEINLINE static Vector2i& Add(const Vector2i& v0, const Vector2i& v1, Vector2i& r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    return r;
  }


  SPC_FORCEINLINE static Vector2i& Add(const Vector2i& v0, int v1, Vector2i& r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    return r;
  }


  SPC_FORCEINLINE static Vector2i& Sub(const Vector2i& v0, const Vector2i& v1, Vector2i& r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    return r;
  }


  SPC_FORCEINLINE static Vector2i& Sub(const Vector2i& v0, int v1, Vector2i& r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    return r;
  }

  SPC_FORCEINLINE static Vector2i& Mul(const Vector2i& v0, const Vector2i& v1, Vector2i& r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    return r;
  }

  SPC_FORCEINLINE static Vector2i& Mul(const Vector2i& v0, int v1, Vector2i& r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    return r;
  }

  SPC_FORCEINLINE static Vector2i& Div(const Vector2i& v0, const Vector2i& v1, Vector2i& r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    return r;
  }

  SPC_FORCEINLINE static Vector2i& Div(const Vector2i& v0, int v1, Vector2i& r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    return r;
  }

  SPC_FORCEINLINE int Dot() const
  {
    return x * x + y * y;
  }

  SPC_FORCEINLINE int Dot(const Vector2i& v) const
  {
    return x * v.x + y * v.y;
  }

  SPC_FORCEINLINE static int Dot(const Vector2i& v0, const Vector2i& v1)
  {
    return v0.x* v1.x + v0.y * v1.y;
  }


};



}

