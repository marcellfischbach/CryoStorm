#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <ceCore/math/math.hh>



namespace ce {



struct CE_CORE_API Vector2i
{
public:
  int x;
  int y;

public:
  CE_FORCEINLINE  Vector2i()
      : x(0)
        , y(0)
  {

  }

  CE_FORCEINLINE explicit Vector2i(const int* f)
      : x(f[0])
        , y(f[1])
  {

  }

  CE_FORCEINLINE Vector2i(const Vector2i& o) = default;

  CE_FORCEINLINE Vector2i(int x, int y)
      : x(x)
        , y(y)
  {

  }

  CE_FORCEINLINE Vector2i& operator=(const Vector2i &v) = default;

  CE_FORCEINLINE Vector2i& operator+=(const Vector2i& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  CE_FORCEINLINE Vector2i& operator+=(int v)
  {
    x += v;
    y += v;
    return *this;
  }

  CE_FORCEINLINE Vector2i& operator-=(const Vector2i& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  CE_FORCEINLINE Vector2i& operator-=(int v)
  {
    x -= v;
    y -= v;
    return *this;
  }


  CE_FORCEINLINE Vector2i& operator*=(int v)
  {
    x *= v;
    y *= v;
    return *this;
  }


  CE_FORCEINLINE Vector2i& operator/=(int v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  CE_FORCEINLINE Vector2i operator-() const
  {
    return Vector2i(-x, -y);
  }

  CE_FORCEINLINE Vector2i operator+() const
  {
    return Vector2i(x, y);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator+(const Vector2i& v0, const Vector2i &v1)
  {
    return Vector2i(v0.x+v1.x,
                    v0.y+v1.y);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator+(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x+v1,
                    v0.y+v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator+(int v0, const Vector2i& v1)
  {
    return Vector2i(v0+v1.x,
                    v0+v1.x);
  }


  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator-(const Vector2i& v0, const Vector2i &v1)
  {
    return Vector2i(v0.x-v1.x,
                    v0.y-v1.y);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator-(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x-v1,
                    v0.y-v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator-(int v0, const Vector2i& v1)
  {
    return Vector2i(v0-v1.x,
                    v0-v1.y);
  }

  CE_NODISCARD CE_FORCEINLINE friend int operator*(const Vector2i& v0, const Vector2i &v1)
  {
    return v0.x * v1.x + v0.y *v1.y;
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator*(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x*v1,
                    v0.y*v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator*(int v0, const Vector2i& v1)
  {
    return Vector2i(v0*v1.x,
                    v0*v1.y);
  }


  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator/(const Vector2i& v0, int v1)
  {
    return Vector2i(v0.x/v1,
                    v0.y/v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2i operator/(int v0, const Vector2i& v1)
  {
    return Vector2i(v0/v1.x,
                    v0/v1.y);
  }

  CE_NODISCARD CE_FORCEINLINE static Vector2i Scale(const Vector2i& v0, const Vector2i& v1)
  {
    return Vector2i(v0.x*v1.x, v0.y*v1.y);
  }


  CE_NODISCARD CE_FORCEINLINE int Dot() const
  {
    return x * x + y * y;
  }

  CE_NODISCARD CE_FORCEINLINE int Dot(const Vector2i& v) const
  {
    return x * v.x + y * v.y;
  }

  CE_NODISCARD CE_FORCEINLINE static int Dot(const Vector2i& v0, const Vector2i& v1)
  {
    return v0.x* v1.x + v0.y * v1.y;
  }


};



}

