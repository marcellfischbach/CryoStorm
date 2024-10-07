#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csDefs.hh>
#include <csCore/math/csVector3f.hh>

namespace cs
{


class CS_CORE_API csSphere
{
public:
  csVector3f center;
  float      radius;

public:
  CS_FORCEINLINE csSphere(const csVector3f& center, float radius)
          : center(center)
          , radius(radius)
  {

  }

  CS_NODISCARD CS_FORCEINLINE float DistanceToCenter(const csVector3f &p) const
  {
    return (p - center).Length();
  }

  CS_NODISCARD CS_FORCEINLINE float DistanceToSurface(const csVector3f &p) const
  {
    return (p - center).Length() - radius;
  }


};


}