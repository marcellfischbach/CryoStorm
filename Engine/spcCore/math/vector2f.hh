#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/math.hh>
#include <cmath>


namespace spc
{


struct SPC_CORE_API Vector2f
{
public:
  float x;
  float y;

public:
  SPC_FORCEINLINE Vector2f()
      : x(0.0f)
        , y(0.0f)
  {

  }

  SPC_FORCEINLINE explicit Vector2f(const float *f)
      : x(f[0])
        , y(f[1])
  {

  }

  SPC_FORCEINLINE Vector2f(const Vector2f &o) = default;

  SPC_FORCEINLINE Vector2f(float x, float y)
      : x(x)
        , y(y)
  {

  }

  SPC_FORCEINLINE Vector2f &operator=(const Vector2f &v) = default;

  SPC_FORCEINLINE Vector2f &operator+=(const Vector2f &v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2f &operator+=(float v)
  {
    x += v;
    y += v;
    return *this;
  }

  SPC_FORCEINLINE Vector2f &operator-=(const Vector2f &v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  SPC_FORCEINLINE Vector2f &operator-=(float v)
  {
    x -= v;
    y -= v;
    return *this;
  }


  SPC_FORCEINLINE Vector2f &operator*=(float v)
  {
    x *= v;
    y *= v;
    return *this;
  }


  SPC_FORCEINLINE Vector2f &operator/=(float v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  SPC_FORCEINLINE Vector2f operator-() const
  {
    return Vector2f(-x, -y);
  }

  SPC_FORCEINLINE Vector2f operator+() const
  {
    return Vector2f(x, y);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator+(const Vector2f &v0, const Vector2f &v1)
  {
    return Vector2f(v0.x + v1.x,
                    v0.y + v1.y);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator+(const Vector2f &v0, float v1)
  {
    return Vector2f(v0.x + v1,
                    v0.y + v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator+(float v0, const Vector2f &v1)
  {
    return Vector2f(v0 + v1.x,
                    v0 + v1.y);
  }


  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator-(const Vector2f &v0, const Vector2f &v1)
  {
    return Vector2f(v0.x - v1.x,
                    v0.y - v1.y);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator-(const Vector2f &v0, float v1)
  {
    return Vector2f(v0.x - v1,
                    v0.y - v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator-(float v0, const Vector2f &v1)
  {
    return Vector2f(v0 - v1.x,
                    v0 - v1.y);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend float operator*(const Vector2f &v0, const Vector2f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y;
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator*(const Vector2f &v0, float v1)
  {
    return Vector2f(v0.x * v1,
                    v0.y * v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator*(float v0, const Vector2f &v1)
  {
    return Vector2f(v0 * v1.x,
                    v0 * v1.y);
  }


  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator/(const Vector2f &v0, float v1)
  {
    return Vector2f(v0.x / v1,
                    v0.y / v1);
  }

  SPC_NODISCARD SPC_FORCEINLINE friend Vector2f operator/(float v0, const Vector2f &v1)
  {
    return Vector2f(v0 / v1.x,
                    v0 / v1.y);
  }

  SPC_NODISCARD SPC_FORCEINLINE static Vector2f Scale(const Vector2f &v0, const Vector2f &v1)
  {
    return Vector2f(v0.x * v1.x, v0.y * v1.y);
  }


  SPC_NODISCARD SPC_FORCEINLINE float Dot() const
  {
    return x * x + y * y;
  }

  SPC_NODISCARD SPC_FORCEINLINE float Dot(const Vector2f &v) const
  {
    return x * v.x + y * v.y;
  }

  SPC_NODISCARD SPC_FORCEINLINE static float Dot(const Vector2f &v0, const Vector2f &v1)
  {
    return v0.x * v1.x + v0.y * v1.y;
  }

  SPC_NODISCARD SPC_FORCEINLINE float Length() const
  {
    return std::sqrt(x * x + y * y);
  }

  SPC_FORCEINLINE Vector2f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    return *this;
  }

  SPC_NODISCARD SPC_FORCEINLINE Vector2f Normalized() const
  {
    float l = Length();
    return Vector2f(x / l, y / l);
  }

};


}

