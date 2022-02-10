
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/math/eclippingresult.hh>
#include <ceCore/math/boundingbox.hh>
#include <ceCore/math/sphere.hh>
#include <ceCore/math/vector3f.hh>

namespace ce
{


struct CE_CORE_API iClipper
{
  virtual ~iClipper() = default;

  CE_NODISCARD virtual eClippingResult Test (const BoundingBox &bbox) const = 0;

  CE_NODISCARD virtual eClippingResult Test (const Sphere &sphere) const = 0;

  CE_NODISCARD virtual eClippingResult Test (const Vector3f &p) const = 0;

};


}