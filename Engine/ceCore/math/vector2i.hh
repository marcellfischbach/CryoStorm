#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <ceCore/math/math.hh>



namespace cryo {



struct CS_CORE_API Vector2i
{
public:
  int x;
  int y;

public:
  CS_FORCEINLINE  Vector2i()
      : x(0)
        , y(0)
  {

  }

  CS_FORCEINLINE explicit Vector2i(const int* f)
      : x(f[0])
        , y(f[1])
  {

  }

  CS_FORCEINLINE Vector2i(const Vector2i& o) = default;

  CS_FORCEINLINE Vector2i(int x, int y)
      : x(x)
        , y(y)
  {

  }

  CS_FORCEINLINE Vector2i& operator=(const Vector2i &v) = default;

  CS_FORCEINLINE Vector2i& operator+=(const Vector2i& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  CS_FORCEINLINE Vector2i& operator+=(int v)
  {
    x += v;
    y += v;
    return *this;
  }

  CS_FORCEINLINE Vector2i& operator-=(const Vector2i& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  CS_FORCEINLINE Vector2i& operator-=(int v)
  {
    x -= v;
    y -= v;
    return *this;
  }


  CS_FORCEINLINE Vector2i& operator*=(int v)
  {
    x *= v;
    y *= v;
    return *this;
  }


  CS_FORCEINLINE Vector2i& operator/=(int v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  CS_FORCEINLINE Vector2i operator-() const
  {
    return Vector2i(-x, -y);
  }

  CS_FORCEINLINE Vector2i operator+() const
  {
    return Vector2i(x, y);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator+(const Vector2i& v0, const Vector2i &v1)
  {
    return Vector2i(v0.x+v1.x,
                    v0.y+v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator+(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x+v1,
                    v0.y+v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator+(int v0, const Vector2i& v1)
  {
    return Vector2i(v0+v1.x,
                    v0+v1.x);
  }


  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator-(const Vector2i& v0, const Vector2i &v1)
  {
    return Vector2i(v0.x-v1.x,
                    v0.y-v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator-(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x-v1,
                    v0.y-v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator-(int v0, const Vector2i& v1)
  {
    return Vector2i(v0-v1.x,
                    v0-v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE friend int operator*(const Vector2i& v0, const Vector2i &v1)
  {
    return v0.x * v1.x + v0.y *v1.y;
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator*(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x*v1,
                    v0.y*v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator*(int v0, const Vector2i& v1)
  {
    return Vector2i(v0*v1.x,
                    v0*v1.y);
  }


  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator/(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x/v1,
                    v0.y/v1);
  }

  CS_NODISCARD CS_FORCEINLINE friend Vector2i operator/(int v0, const Vector2i& v1)
  {
    return Vector2i(v0/v1.x,
                    v0/v1.y);
  }

  CS_NODISCARD CS_FORCEINLINE static Vector2i Scale(const Vector2i& v0, const Vector2i& v1)
  {
    return Vector2i(v0.x*v1.x, v0.y*v1.y);
  }


  CS_NODISCARD CS_FORCEINLINE int Dot() const
  {
    return x * x + y * y;
  }

  CS_NODISCARD CS_FORCEINLINE int Dot(const Vector2i& v) const
  {
    return x * v.x + y * v.y;
  }

  CS_NODISCARD CS_FORCEINLINE static int Dot(const Vector2i& v0, const Vector2i& v1)
  {
    return v0.x* v1.x + v0.y * v1.y;
  }


};



}

