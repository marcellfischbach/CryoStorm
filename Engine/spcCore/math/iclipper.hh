
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/math/eclippingresult.hh>
#include <spcCore/math/boundingbox.hh>
#include <spcCore/math/sphere.hh>
#include <spcCore/math/vector3f.hh>

namespace spc
{


struct SPC_CORE_API iClipper
{
  virtual ~iClipper() = default;

  SPC_NODISCARD virtual eClippingResult Test (const BoundingBox &bbox) const = 0;

  SPC_NODISCARD virtual eClippingResult Test (const Sphere &sphere) const = 0;

  SPC_NODISCARD virtual eClippingResult Test (const Vector3f &p) const = 0;

};


}