#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <ceCore/math/vector3f.hh>

namespace ce
{


class CE_CORE_API Sphere
{
public:
  Vector3f center;
  float radius;

public:
  CE_FORCEINLINE Sphere(const Vector3f& center, float radius)
          : center(center)
          , radius(radius)
  {

  }

  CE_NODISCARD CE_FORCEINLINE float DistanceToCenter(const Vector3f &p) const
  {
    return (p - center).Length();
  }

  CE_NODISCARD CE_FORCEINLINE float DistanceToSurface(const Vector3f &p) const
  {
    return (p - center).Length() - radius;
  }


};


}