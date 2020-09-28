#pragma once

#include <SpiceCore/coreexport.hh>
#include <SpiceCore/math/vector3f.hh>



namespace Spice {


struct SPICE_CORE_API Plane
{
public:
  float x;
  float y;
  float z;
  float d;

public:
  SPICE_FORCEINLINE Plane(float x = 0.0f, float y = 0.0f, float z = 0.0f, float d = 0.0f)
    : x(x)
    , y(y)
    , z(z)
    , d(d)
  {

  }

  SPICE_FORCEINLINE Plane(const Vector3f& pos, const Vector3f& norm)
  {
    Vector3f nn;
    norm.Normalized(nn);
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -pos.Dot(nn);
  }

  SPICE_FORCEINLINE float Distance(const Vector3f& pos) const
  {
    return x * pos.x + y * pos.y + z * pos.z + d;
  }


  SPICE_FORCEINLINE Vector3f& ReflectPoint(const Vector3f& p, Vector3f& r) const
  {
    Vector3f origin, diff;
    Vector3f normal = Vector3f(x, y, z);
    Vector3f::Mul(normal, -d, origin);
    Vector3f::Sub(p, origin, diff);

    Vector3f::Mul(normal, 2.0f * diff.Dot(normal), r);
    Vector3f::Sub(diff, r, r);
    return Vector3f::Add(r, origin, r);
  }

  Vector3f& ReflectDirection(const Vector3f& d, Vector3f& r) const
  {
    Vector3f normal = Vector3f(x, y, z);
    Vector3f::Mul(normal, 2.0f * d.Dot(normal), r);
    return Vector3f::Sub(d, r, r);
  }


};



}

