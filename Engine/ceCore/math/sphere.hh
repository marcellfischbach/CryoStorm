#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <ceCore/math/vector3f.hh>

namespace cryo
{


class CS_CORE_API Sphere
{
public:
  Vector3f center;
  float radius;

public:
  CS_FORCEINLINE Sphere(const Vector3f& center, float radius)
          : center(center)
          , radius(radius)
  {

  }

  CS_NODISCARD CS_FORCEINLINE float DistanceToCenter(const Vector3f &p) const
  {
    return (p - center).Length();
  }

  CS_NODISCARD CS_FORCEINLINE float DistanceToSurface(const Vector3f &p) const
  {
    return (p - center).Length() - radius;
  }


};


}