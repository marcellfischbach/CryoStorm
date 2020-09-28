#pragma once

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/defs.hh>
#include <SpiceCore/math/vector2f.hh>
#include <SpiceCore/math/vector3f.hh>
#include <math.h>
#include <stdio.h>




namespace Spice {



struct SPICE_CORE_API Vector4f
{
public:
  float x;
  float y;
  float z;
  float w;

public:
  SPICE_FORCEINLINE Vector4f(const float* f)
    : x(f[0])
    , y(f[1])
    , z(f[2])
    , w(f[3])
  {

  }

  SPICE_FORCEINLINE Vector4f(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
  {

  }

  SPICE_FORCEINLINE Vector4f(const Vector3f& v, float w = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(w)
  {

  }

  SPICE_FORCEINLINE Vector4f(const Vector2f& v, float z = 0.0f, float w = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(z)
    , w(w)
  {

  }

  SPICE_FORCEINLINE Vector4f(const Vector2f& v0, const Vector2f& v1)
    : x(v0.x)
    , y(v0.y)
    , z(v1.x)
    , w(v1.y)
  {

  }


  SPICE_FORCEINLINE Vector2f AsVector2f() const
  {
    return Vector2f(x, y);
  }

  SPICE_FORCEINLINE Vector2f& AsVector2f(Vector2f& v) const
  {
    v.x = x;
    v.y = y;
    return v;
  }

  SPICE_FORCEINLINE Vector2f AsVector2fHigh() const
  {
    return Vector2f(z, w);
  }

  SPICE_FORCEINLINE Vector2f& AsVector2fHigh(Vector2f& v) const
  {
    v.x = z;
    v.y = w;
    return v;
  }


  SPICE_FORCEINLINE Vector3f AsVector3f() const
  {
    return Vector3f(x, y, z);
  }

  SPICE_FORCEINLINE Vector3f& AsVector3f(Vector3f& v) const
  {
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
  }



  SPICE_FORCEINLINE Vector4f& operator+=(const Vector4f& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  SPICE_FORCEINLINE Vector4f& operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    w += v;
    return *this;
  }

  SPICE_FORCEINLINE Vector4f& operator-=(const Vector4f& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  SPICE_FORCEINLINE Vector4f& operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return *this;
  }

  SPICE_FORCEINLINE Vector4f& operator*=(const Vector4f& v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
  }

  SPICE_FORCEINLINE Vector4f& operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
  }

  SPICE_FORCEINLINE Vector4f& operator/=(const Vector4f& v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
  }

  SPICE_FORCEINLINE Vector4f& operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
  }

  SPICE_FORCEINLINE static Vector4f& Add(const Vector4f& v0, const Vector4f& v1, Vector4f& r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    r.z = v0.z + v1.z;
    r.w = v0.w + v1.w;
    return r;
  }

  SPICE_FORCEINLINE static Vector4f& Add(const Vector4f& v0, float v1, Vector4f& r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    r.z = v0.z + v1;
    r.w = v0.w + v1;
    return r;
  }

  SPICE_FORCEINLINE static Vector4f& Sub(const Vector4f& v0, const Vector4f& v1, Vector4f& r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    r.z = v0.z - v1.z;
    r.w = v0.w - v1.w;
    return r;
  }

  SPICE_FORCEINLINE static Vector4f& Sub(const Vector4f& v0, float v1, Vector4f& r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    r.z = v0.z - v1;
    r.w = v0.w - v1;
    return r;
  }
  SPICE_FORCEINLINE static Vector4f& Mul(const Vector4f& v0, const Vector4f& v1, Vector4f& r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    r.z = v0.z * v1.z;
    r.w = v0.w * v1.w;
    return r;
  }

  SPICE_FORCEINLINE static Vector4f& Mul(const Vector4f& v0, float v1, Vector4f& r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    r.z = v0.z * v1;
    r.w = v0.w * v1;
    return r;
  }

  SPICE_FORCEINLINE static Vector4f& Div(const Vector4f& v0, const Vector4f& v1, Vector4f& r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    r.z = v0.z / v1.z;
    r.w = v0.w / v1.w;
    return r;
  }

  SPICE_FORCEINLINE static Vector4f& Div(const Vector4f& v0, float v1, Vector4f& r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    r.z = v0.z / v1;
    r.w = v0.w / v1;
    return r;
  }

  SPICE_FORCEINLINE float Dot() const
  {
    return x * x + y * y + z * z + w * w;
  }

  SPICE_FORCEINLINE float Dot(const Vector4f& v) const
  {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  SPICE_FORCEINLINE static float Dot(const Vector4f& v0, const Vector4f& v1)
  {
    return v0.x* v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }

  SPICE_FORCEINLINE float Length() const
  {
    return (float)sqrt(x * x + y * y + z * z + w * w);
  }

  SPICE_FORCEINLINE Vector4f& Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
  }

  SPICE_FORCEINLINE Vector4f& Normalized(Vector4f & r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    r.w = w / l;
    return r;
  }


  SPICE_FORCEINLINE bool operator== (const Vector4f & o) const
  {
    return x == o.x && y == o.y && z == o.z && w == o.w;
  }

  SPICE_FORCEINLINE bool operator!= (const Vector4f & o) const
  {
    return !(x == o.x && y == o.y && z == o.z && w == o.w);
  }


  SPICE_FORCEINLINE void Debug(const char* message)
  {
    printf("ceVector3f ");
    if (message)
    {
      printf("%s ", message);
    }
    printf("<%f %f %f %f>\n", x, y, z, w);
  }
};


}

