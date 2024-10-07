
#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/math/iclipper.hh>
#include <csCore/csDefs.hh>

namespace cs
{


class CS_CORE_API csSphereClipper : public iClipper
{
public:
  csSphereClipper(const csVector3f &center, float radius);
  ~csSphereClipper() override = default;

  CS_NODISCARD eClippingResult Test(const csBoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const csSphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const csVector3f& p) const override;

private:

  csVector3f m_center;
  float      m_radius;
  float      m_radiusSqr;
};


}