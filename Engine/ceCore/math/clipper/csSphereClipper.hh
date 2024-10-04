
#pragma  once

#include <ceCore/csCoreExport.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/csDefs.hh>

namespace cryo
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