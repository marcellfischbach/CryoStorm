#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/vector2f.hh>
#include <math.h>
#include <stdio.h>



namespace spc {


struct SPC_CORE_API Vector3f
{
public:
  float x;
  float y;
  float z;

public:
  SPC_FORCEINLINE Vector3f(const float* f)
    : x(f[0])
    , y(f[1])
    , z(f[2])
  {

  }

  SPC_FORCEINLINE Vector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    : x(x)
    , y(y)
    , z(z)
  {

  }

  SPC_FORCEINLINE Vector3f(const Vector2f& v, float z = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(z)
  {

  }

  SPC_FORCEINLINE void Set(float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  SPC_FORCEINLINE void Set(const Vector3f& v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }

  SPC_FORCEINLINE Vector2f AsVector2f() const
  {
    return Vector2f(x, y);
  }

  SPC_FORCEINLINE Vector2f& AsVector2f(Vector2f& v) const
  {
    v.x = x;
    v.y = y;
    return v;
  }

  SPC_FORCEINLINE Vector3f& operator+=(const Vector3f& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  SPC_FORCEINLINE Vector3f& operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    return *this;
  }

  SPC_FORCEINLINE Vector3f& operator-=(const Vector3f& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  SPC_FORCEINLINE Vector3f& operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    return *this;
  }

  SPC_FORCEINLINE Vector3f& operator*=(const Vector3f& v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  SPC_FORCEINLINE Vector3f& operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    return *this;
  }

  SPC_FORCEINLINE Vector3f& operator/=(const Vector3f& v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
  }

  SPC_FORCEINLINE Vector3f& operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    return *this;
  }

  SPC_FORCEINLINE Vector3f operator*(float v) const
  {
    return Vector3f(x * v, y * v, z * v);
  }

  SPC_FORCEINLINE Vector3f operator/(float v) const
  {
    return Vector3f(x / v, y / v, z / v);
  }

  SPC_FORCEINLINE Vector3f operator+(const Vector3f & o) const
  {
    return Vector3f(x + o.x, y + o.y, z + o.z);
  }

  SPC_FORCEINLINE Vector3f operator-(const Vector3f & o) const
  {
    return Vector3f(x - o.x, y - o.y, z - o.z);
  }

  SPC_FORCEINLINE static Vector3f& Add(const Vector3f & v0, const Vector3f & v1, Vector3f & r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    r.z = v0.z + v1.z;
    return r;
  }

  SPC_FORCEINLINE static Vector3f& Add(const Vector3f & v0, float v1, Vector3f & r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    r.z = v0.z + v1;
    return r;
  }

  SPC_FORCEINLINE static Vector3f& Sub(const Vector3f & v0, const Vector3f & v1, Vector3f & r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    r.z = v0.z - v1.z;
    return r;
  }

  SPC_FORCEINLINE static Vector3f& Sub(const Vector3f & v0, float v1, Vector3f & r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    r.z = v0.z - v1;
    return r;
  }
  SPC_FORCEINLINE static Vector3f& Mul(const Vector3f & v0, const Vector3f & v1, Vector3f & r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    r.z = v0.z * v1.z;
    return r;
  }

  SPC_FORCEINLINE static Vector3f& Mul(const Vector3f & v0, float v1, Vector3f & r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    r.z = v0.z * v1;
    return r;
  }


  SPC_FORCEINLINE static Vector3f& MulAdd(const Vector3f & v0, const Vector3f v1, float fact, Vector3f & r)
  {
    r.x = v0.x + v1.x * fact;
    r.y = v0.y + v1.y * fact;
    r.z = v0.z + v1.z * fact;
    return r;
  }

  SPC_FORCEINLINE static Vector3f& Div(const Vector3f & v0, const Vector3f & v1, Vector3f & r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    r.z = v0.z / v1.z;
    return r;
  }

  SPC_FORCEINLINE static Vector3f& Div(const Vector3f & v0, float v1, Vector3f & r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    r.z = v0.z / v1;
    return r;
  }

  SPC_FORCEINLINE float Dot() const
  {
    return x * x + y * y + z * z;
  }

  SPC_FORCEINLINE float Dot(const Vector3f & v) const
  {
    return x * v.x + y * v.y + z * v.z;
  }

  SPC_FORCEINLINE static float Dot(const Vector3f & v0, const Vector3f & v1)
  {
    return v0.x* v1.x + v0.y * v1.y + v0.z * v1.z;
  }

  SPC_FORCEINLINE float Length() const
  {
    return (float)sqrt(x * x + y * y + z * z);
  }

  SPC_FORCEINLINE Vector3f& Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    return *this;
  }

  SPC_FORCEINLINE Vector3f& Normalized(Vector3f & r) const
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    return r;
  }


  SPC_FORCEINLINE static Vector3f& Cross(const Vector3f & v0, const Vector3f & v1, Vector3f & r)
  {
    r.Set(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x);
    return r;
  }

  SPC_FORCEINLINE void Debug(const char* message)
  {
    printf("ceVector3f ");
    if (message)
    {
      printf("%s ", message);
    }
    printf("<%.2f %.2f %.2f>\n", x, y, z);
  }
};


}

