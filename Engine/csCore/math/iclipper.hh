
#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/math/eClippingResult.hh>
#include <csCore/math/csBoundingBox.hh>
#include <csCore/math/csSphere.hh>
#include <csCore/math/csVector3f.hh>

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