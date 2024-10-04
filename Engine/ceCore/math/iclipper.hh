
#pragma  once

#include <ceCore/csCoreExport.hh>
#include <ceCore/math/eClippingResult.hh>
#include <ceCore/math/csBoundingBox.hh>
#include <ceCore/math/csSphere.hh>
#include <ceCore/math/csVector3f.hh>

namespace cryo
{


struct CS_CORE_API iClipper
{
  virtual ~iClipper() = default;

  CS_NODISCARD virtual eClippingResult Test(const csBoundingBox &bbox) const = 0;

  CS_NODISCARD virtual eClippingResult Test(const csSphere &sphere) const = 0;

  CS_NODISCARD virtual eClippingResult Test(const csVector3f &p) const = 0;

};


}