#pragma once

#include <cecore/ceexport.hh>
#include <cecore/cedefs.hh>
#include <cecore/math/cevector2f.hh>
#include <math.h>
#include <stdio.h>


struct CEC_API ceVector3f
{
public:
  float x;
  float y;
  float z;

public:
  CE_FORCEINLINE ceVector3f(const float* f)
    : x(f[0])
    , y(f[1])
    , z(f[2])
  {

  }

  CE_FORCEINLINE ceVector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    : x(x)
    , y(y)
    , z(z)
  {

  }

  CE_FORCEINLINE ceVector3f(const ceVector2f& v, float z = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(z)
  {

  }

  CE_FORCEINLINE void Set(float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  CE_FORCEINLINE void Set(const ceVector3f& v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }

  CE_FORCEINLINE ceVector2f AsVector2f() const
  {
    return ceVector2f(x, y);
  }

  CE_FORCEINLINE ceVector2f& AsVector2f(ceVector2f& v) const
  {
    v.x = x;
    v.y = y;
    return v;
  }

  CE_FORCEINLINE ceVector3f& operator+=(const ceVector3f& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  CE_FORCEINLINE ceVector3f& operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    return *this;
  }

  CE_FORCEINLINE ceVector3f& operator-=(const ceVector3f& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  CE_FORCEINLINE ceVector3f& operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    return *this;
  }

  CE_FORCEINLINE ceVector3f& operator*=(const ceVector3f& v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  CE_FORCEINLINE ceVector3f& operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    return *this;
  }

  CE_FORCEINLINE ceVector3f& operator/=(const ceVector3f& v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
  }

  CE_FORCEINLINE ceVector3f& operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    return *this;
  }

  CE_FORCEINLINE ceVector3f operator*(float v) const
  {
    return ceVector3f(x * v, y * v, z * v);
  }

  CE_FORCEINLINE ceVector3f operator/(float v) const
  {
    return ceVector3f(x / v, y / v, z / v);
  }

  CE_FORCEINLINE ceVector3f operator+(const ceVector3f & o) const
  {
    return ceVector3f(x + o.x, y + o.y, z + o.z);
  }

  CE_FORCEINLINE ceVector3f operator-(const ceVector3f & o) const
  {
    return ceVector3f(x - o.x, y - o.y, z - o.z);
  }

  CE_FORCEINLINE static ceVector3f& Add(const ceVector3f & v0, const ceVector3f & v1, ceVector3f & r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    r.z = v0.z + v1.z;
    return r;
  }

  CE_FORCEINLINE static ceVector3f& Add(const ceVector3f & v0, float v1, ceVector3f & r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    r.z = v0.z + v1;
    return r;
  }

  CE_FORCEINLINE static ceVector3f& Sub(const ceVector3f & v0, const ceVector3f & v1, ceVector3f & r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    r.z = v0.z - v1.z;
    return r;
  }

  CE_FORCEINLINE static ceVector3f& Sub(const ceVector3f & v0, float v1, ceVector3f & r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    r.z = v0.z - v1;
    return r;
  }
  CE_FORCEINLINE static ceVector3f& Mul(const ceVector3f & v0, const ceVector3f & v1, ceVector3f & r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    r.z = v0.z * v1.z;
    return r;
  }

  CE_FORCEINLINE static ceVector3f& Mul(const ceVector3f & v0, float v1, ceVector3f & r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    r.z = v0.z * v1;
    return r;
  }


  CE_FORCEINLINE static ceVector3f& MulAdd(const ceVector3f & v0, const ceVector3f v1, float fact, ceVector3f & r)
  {
    r.x = v0.x + v1.x * fact;
    r.y = v0.y + v1.y * fact;
    r.z = v0.z + v1.z * fact;
    return r;
  }

  CE_FORCEINLINE static ceVector3f& Div(const ceVector3f & v0, const ceVector3f & v1, ceVector3f & r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    r.z = v0.z / v1.z;
    return r;
  }

  CE_FORCEINLINE static ceVector3f& Div(const ceVector3f & v0, float v1, ceVector3f & r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    r.z = v0.z / v1;
    return r;
  }

  CE_FORCEINLINE float Dot() const
  {
    return x * x + y * y + z * z;
  }

  CE_FORCEINLINE float Dot(const ceVector3f & v) const
  {
    return x * v.x + y * v.y + z * v.z;
  }

  CE_FORCEINLINE static float Dot(const ceVector3f & v0, const ceVector3f & v1)
  {
    return v0.x* v1.x + v0.y * v1.y + v0.z * v1.z;
  }

  CE_FORCEINLINE float Length() const
  {
    return (float)sqrt(x * x + y * y + z * z);
  }

  CE_FORCEINLINE ceVector3f& Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    return *this;
  }

  CE_FORCEINLINE ceVector3f& Normalized(ceVector3f & r) const
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    return r;
  }


  CE_FORCEINLINE static ceVector3f& Cross(const ceVector3f & v0, const ceVector3f & v1, ceVector3f & r)
  {
    r.Set(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x);
    return r;
  }

  CE_FORCEINLINE void Debug(const char* message)
  {
    printf("ceVector3f ");
    if (message)
    {
      printf("%s ", message);
    }
    printf("<%.2f %.2f %.2f>\n", x, y, z);
  }
};

