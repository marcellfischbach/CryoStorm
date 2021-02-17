#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/vector3f.hh>

namespace spc
{


class SPC_CORE_API Sphere
{
public:
  Vector3f center;
  float radius;

public:
  SPC_FORCEINLINE Sphere(const Vector3f& center, float radius)
          : center(center)
          , radius(radius)
  {

  }

  SPC_NODISCARD SPC_FORCEINLINE float DistanceToCenter(const Vector3f &p) const
  {
    return (p - center).Length();
  }

  SPC_NODISCARD SPC_FORCEINLINE float DistanceToSurface(const Vector3f &p) const
  {
    return (p - center).Length() - radius;
  }


};


}