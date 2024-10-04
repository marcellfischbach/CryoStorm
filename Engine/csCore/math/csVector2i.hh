#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include "csMath.hh"



namespace cryo {



struct CS_CORE_API csVector2i
{
public:
  int x;
  int y;

public:
  CS_FORCEINLINE  csVector2i()
      : x(0)
        , y(0)
  {

  }

  CS_FORCEINLINE explicit csVector2i(const int* f)
      : x(f[0])
        , y(f[1])
  {

  }

  CS_FORCEINLINE csVector2i(const csVector2i& o) = default;

  CS_FORCEINLINE csVector2i(int x, int y)
      : x(x)
        , y(y)
  {

  }

  CS_FORCEINLINE csVector2i& operator=(const csVector2i &v) = default;

  CS_FORCEINLINE csVector2i& operator+=(const csVector2i& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  CS_FORCEINLINE csVector2i& operator+=(int v)
  {
    x += v;
    y += v;
    return *this;
  }

  CS_FORCEINLINE csVector2i& operator-=(const csVector2i& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  CS_FORCEINLINE csVector2i& operator-=(int v)
  {
    x -= v;
    y -= v;
    return *this;
  }


  CS_FORCEINLINE csVector2i& operator*=(int v)
  {
    x *= v;
    y *= v;
    return *this;
  }


  CS_FORCEINLINE csVector2i& operator/=(int v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  CS_FORCEINLINE csVector2i operator-() const
  {
    return csVector2i(-x, -y);
  }

  CS_FORCEINLINE csVector2i operator+() const
  {
    return csVector2i(x, y);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator+(const csVector2i& v0, const csVector2i &v1)
  {
    return csVector2i(v0.x + v1.x,
                    v0.y+v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator+(const csVector2i& v0, int v1)
  {
    return csVector2i(v0.x + v1,
                    v0.y+v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator+(int v0, const csVector2i& v1)
  {
    return csVector2i(v0 + v1.x,
                    v0+v1.x);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator-(const csVector2i& v0, const csVector2i &v1)
  {
    return csVector2i(v0.x - v1.x,
                    v0.y-v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator-(const csVector2i& v0, int v1)
  {
    return csVector2i(v0.x - v1,
                    v0.y-v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator-(int v0, const csVector2i& v1)
  {
    return csVector2i(v0 - v1.x,
                    v0-v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE friend int operator*(const csVector2i& v0, const csVector2i &v1)
  {
    return v0.x * v1.x + v0.y *v1.y;
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator*(const csVector2i& v0, int v1)
  {
    return csVector2i(v0.x * v1,
                    v0.y*v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator*(int v0, const csVector2i& v1)
  {
    return csVector2i(v0 * v1.x,
                    v0*v1.y);
  }


  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator/(const csVector2i& v0, int v1)
  {
    return csVector2i(v0.x / v1,
                    v0.y/v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend csVector2i operator/(int v0, const csVector2i& v1)
  {
    return csVector2i(v0 / v1.x,
                    v0/v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE static csVector2i Scale(const csVector2i& v0, const csVector2i& v1)
  {
    return csVector2i(v0.x * v1.x, v0.y * v1.y);
  }


  CS_NODISCARD CS_FORCEINLINE int Dot() const
  {
    return x * x + y * y;
  }

  CS_NODISCARD CS_FORCEINLINE int Dot(const csVector2i& v) const
  {
    return x * v.x + y * v.y;
  }

  CS_NODISCARD CS_FORCEINLINE static int Dot(const csVector2i& v0, const csVector2i& v1)
  {
    return v0.x* v1.x + v0.y * v1.y;
  }


};



}

