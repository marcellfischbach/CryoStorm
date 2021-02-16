
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/math/iclipper.hh>
#include <spcCore/defs.hh>

namespace spc
{


class SPC_CORE_API SphereClipper : public iClipper
{
public:
  SphereClipper(const Vector3f &center, float radius);
  ~SphereClipper() override = default;

  SPC_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  SPC_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  SPC_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  Vector3f m_center;
  float m_radius;
  float m_radiusSqr;
};


}