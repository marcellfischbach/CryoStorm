#pragma once

#include <cecore/ceexport.hh>
#include <cecore/cedefs.hh>
#include <cecore/math/cevector2f.hh>
#include <cecore/math/cevector3f.hh>
#include <math.h>
#include <stdio.h>




struct CEC_API ceVector4f
{
public:
  float x;
  float y;
  float z;
  float w;

public:
  CE_FORCEINLINE ceVector4f(const float* f)
    : x(f[0])
    , y(f[1])
    , z(f[2])
    , w(f[3])
  {

  }

  CE_FORCEINLINE ceVector4f(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
  {

  }

  CE_FORCEINLINE ceVector4f(const ceVector3f& v, float w = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(w)
  {

  }

  CE_FORCEINLINE ceVector4f(const ceVector2f& v, float z = 0.0f, float w = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(z)
    , w(w)
  {

  }

  CE_FORCEINLINE ceVector4f(const ceVector2f& v0, const ceVector2f& v1)
    : x(v0.x)
    , y(v0.y)
    , z(v1.x)
    , w(v1.y)
  {

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

  CE_FORCEINLINE ceVector2f AsVector2fHigh() const
  {
    return ceVector2f(z, w);
  }

  CE_FORCEINLINE ceVector2f& AsVector2fHigh(ceVector2f& v) const
  {
    v.x = z;
    v.y = w;
    return v;
  }


  CE_FORCEINLINE ceVector3f AsVector3f() const
  {
    return ceVector3f(x, y, z);
  }

  CE_FORCEINLINE ceVector3f& AsVector3f(ceVector3f& v) const
  {
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
  }



  CE_FORCEINLINE ceVector4f& operator+=(const ceVector4f& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  CE_FORCEINLINE ceVector4f& operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    w += v;
    return *this;
  }

  CE_FORCEINLINE ceVector4f& operator-=(const ceVector4f& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  CE_FORCEINLINE ceVector4f& operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return *this;
  }

  CE_FORCEINLINE ceVector4f& operator*=(const ceVector4f& v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
  }

  CE_FORCEINLINE ceVector4f& operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
  }

  CE_FORCEINLINE ceVector4f& operator/=(const ceVector4f& v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
  }

  CE_FORCEINLINE ceVector4f& operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
  }

  CE_FORCEINLINE static ceVector4f& Add(const ceVector4f& v0, const ceVector4f& v1, ceVector4f& r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    r.z = v0.z + v1.z;
    r.w = v0.w + v1.w;
    return r;
  }

  CE_FORCEINLINE static ceVector4f& Add(const ceVector4f& v0, float v1, ceVector4f& r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    r.z = v0.z + v1;
    r.w = v0.w + v1;
    return r;
  }

  CE_FORCEINLINE static ceVector4f& Sub(const ceVector4f& v0, const ceVector4f& v1, ceVector4f& r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    r.z = v0.z - v1.z;
    r.w = v0.w - v1.w;
    return r;
  }

  CE_FORCEINLINE static ceVector4f& Sub(const ceVector4f& v0, float v1, ceVector4f& r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    r.z = v0.z - v1;
    r.w = v0.w - v1;
    return r;
  }
  CE_FORCEINLINE static ceVector4f& Mul(const ceVector4f& v0, const ceVector4f& v1, ceVector4f& r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    r.z = v0.z * v1.z;
    r.w = v0.w * v1.w;
    return r;
  }

  CE_FORCEINLINE static ceVector4f& Mul(const ceVector4f& v0, float v1, ceVector4f& r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    r.z = v0.z * v1;
    r.w = v0.w * v1;
    return r;
  }

  CE_FORCEINLINE static ceVector4f& Div(const ceVector4f& v0, const ceVector4f& v1, ceVector4f& r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    r.z = v0.z / v1.z;
    r.w = v0.w / v1.w;
    return r;
  }

  CE_FORCEINLINE static ceVector4f& Div(const ceVector4f& v0, float v1, ceVector4f& r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    r.z = v0.z / v1;
    r.w = v0.w / v1;
    return r;
  }

  CE_FORCEINLINE float Dot() const
  {
    return x * x + y * y + z * z + w * w;
  }

  CE_FORCEINLINE float Dot(const ceVector4f& v) const
  {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  CE_FORCEINLINE static float Dot(const ceVector4f& v0, const ceVector4f& v1)
  {
    return v0.x* v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

  CE_FORCEINLINE float Length() const
  {
    return (float)sqrt(x * x + y * y + z * z + w * w);
  }

  CE_FORCEINLINE ceVector4f& Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
  }

  CE_FORCEINLINE ceVector4f& Normalized(ceVector4f & r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    r.w = w / l;
    return r;
  }


  CE_FORCEINLINE bool operator== (const ceVector4f & o) const
  {
    return x == o.x && y == o.y && z == o.z && w == o.w;
  }

  CE_FORCEINLINE bool operator!= (const ceVector4f & o) const
  {
    return !(x == o.x && y == o.y && z == o.z && w == o.w);
  }


  CE_FORCEINLINE void Debug(const char* message)
  {
    printf("ceVector3f ");
    if (message)
    {
      printf("%s ", message);
    }
    printf("<%f %f %f %f>\n", x, y, z, w);
  }
};

