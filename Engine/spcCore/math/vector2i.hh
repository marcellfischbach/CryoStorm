#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/math.hh>



namespace spc {



struct SPC_CORE_API Vector2i
{
public:
  int x;
  int y;

public:
  SPC_FORCEINLINE  Vector2i()
      : x(0)
        , y(0)
  {

  }

  SPC_FORCEINLINE explicit Vector2i(const int* f)
      : x(f[0])
        , y(f[1])
  {

  }

  SPC_FORCEINLINE Vector2i(const Vector2i& o) = default;

  SPC_FORCEINLINE Vector2i(int x, int y)
      : x(x)
        , y(y)
  {

  }

  SPC_FORCEINLINE Vector2i& operator=(const Vector2i &v) = default;

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


  SPC_FORCEINLINE Vector2i& operator*=(int v)
  {
    x *= v;
    y *= v;
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

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator+(const Vector2i& v0, const Vector2i &v1)
  {
    return Vector2i(v0.x+v1.x,
                    v0.y+v1.y);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator+(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x+v1,
                    v0.y+v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator+(int v0, const Vector2i& v1)
  {
    return Vector2i(v0+v1.x,
                    v0+v1.x);
  }


  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator-(const Vector2i& v0, const Vector2i &v1)
  {
    return Vector2i(v0.x-v1.x,
                    v0.y-v1.y);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator-(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x-v1,
                    v0.y-v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator-(int v0, const Vector2i& v1)
  {
    return Vector2i(v0-v1.x,
                    v0-v1.y);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend int operator*(const Vector2i& v0, const Vector2i &v1)
  {
    return v0.x * v1.x + v0.y *v1.y;
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator*(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x*v1,
                    v0.y*v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator*(int v0, const Vector2i& v1)
  {
    return Vector2i(v0*v1.x,
                    v0*v1.y);
  }


  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator/(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x/v1,
                    v0.y/v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2i operator/(int v0, const Vector2i& v1)
  {
    return Vector2i(v0/v1.x,
                    v0/v1.y);
  }

  SPC_NODISCARD SPC_FORCEINLINE static Vector2i Scale(const Vector2i& v0, const Vector2i& v1)
  {
    return Vector2i(v0.x*v1.x, v0.y*v1.y);
  }


  SPC_NODISCARD SPC_FORCEINLINE int Dot() const
  {
    return x * x + y * y;
  }

  SPC_NODISCARD SPC_FORCEINLINE int Dot(const Vector2i& v) const
  {
    return x * v.x + y * v.y;
  }

  SPC_NODISCARD SPC_FORCEINLINE static int Dot(const Vector2i& v0, const Vector2i& v1)
  {
    return v0.x* v1.x + v0.y * v1.y;
  }


};



}

