#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <ceCore/math/math.hh>
#include <cmath>


namespace ce
{


struct CE_CORE_API Vector2f
{
public:
  float x;
  float y;

public:
  CE_FORCEINLINE Vector2f()
      : x(0.0f)
        , y(0.0f)
  {

  }

  CE_FORCEINLINE explicit Vector2f(const float *f)
      : x(f[0])
        , y(f[1])
  {

  }

  CE_FORCEINLINE Vector2f(const Vector2f &o) = default;

  CE_FORCEINLINE Vector2f(float x, float y)
      : x(x)
        , y(y)
  {

  }

  CE_FORCEINLINE Vector2f &operator=(const Vector2f &v) = default;

  CE_FORCEINLINE Vector2f &operator+=(const Vector2f &v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  CE_FORCEINLINE Vector2f &operator+=(float v)
  {
    x += v;
    y += v;
    return *this;
  }

  CE_FORCEINLINE Vector2f &operator-=(const Vector2f &v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  CE_FORCEINLINE Vector2f &operator-=(float v)
  {
    x -= v;
    y -= v;
    return *this;
  }


  CE_FORCEINLINE Vector2f &operator*=(float v)
  {
    x *= v;
    y *= v;
    return *this;
  }


  CE_FORCEINLINE Vector2f &operator/=(float v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  CE_FORCEINLINE Vector2f operator-() const
  {
    return Vector2f(-x, -y);
  }

  CE_FORCEINLINE Vector2f operator+() const
  {
    return Vector2f(x, y);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator+(const Vector2f &v0, const Vector2f &v1)
  {
    return Vector2f(v0.x + v1.x,
                    v0.y + v1.y);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator+(const Vector2f &v0, float v1)
  {
    return Vector2f(v0.x + v1,
                    v0.y + v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator+(float v0, const Vector2f &v1)
  {
    return Vector2f(v0 + v1.x,
                    v0 + v1.y);
  }


  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator-(const Vector2f &v0, const Vector2f &v1)
  {
    return Vector2f(v0.x - v1.x,
                    v0.y - v1.y);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator-(const Vector2f &v0, float v1)
  {
    return Vector2f(v0.x - v1,
                    v0.y - v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator-(float v0, const Vector2f &v1)
  {
    return Vector2f(v0 - v1.x,
                    v0 - v1.y);
  }

  CE_NODISCARD CE_FORCEINLINE friend float operator*(const Vector2f &v0, const Vector2f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y;
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator*(const Vector2f &v0, float v1)
  {
    return Vector2f(v0.x * v1,
                    v0.y * v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator*(float v0, const Vector2f &v1)
  {
    return Vector2f(v0 * v1.x,
                    v0 * v1.y);
  }


  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator/(const Vector2f &v0, float v1)
  {
    return Vector2f(v0.x / v1,
                    v0.y / v1);
  }

  CE_NODISCARD CE_FORCEINLINE friend Vector2f operator/(float v0, const Vector2f &v1)
  {
    return Vector2f(v0 / v1.x,
                    v0 / v1.y);
  }

  CE_NODISCARD CE_FORCEINLINE static Vector2f Scale(const Vector2f &v0, const Vector2f &v1)
  {
    return Vector2f(v0.x * v1.x, v0.y * v1.y);
  }


  CE_NODISCARD CE_FORCEINLINE float Dot() const
  {
    return x * x + y * y;
  }

  CE_NODISCARD CE_FORCEINLINE float Dot(const Vector2f &v) const
  {
    return x * v.x + y * v.y;
  }

  CE_NODISCARD CE_FORCEINLINE static float Dot(const Vector2f &v0, const Vector2f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y;
  }

  CE_NODISCARD CE_FORCEINLINE float Length() const
  {
    return std::sqrt(x * x + y * y);
  }

  CE_FORCEINLINE Vector2f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    return *this;
  }

  CE_NODISCARD CE_FORCEINLINE Vector2f Normalized() const
  {
    float l = Length();
    return Vector2f(x / l, y / l);
  }

};


}

