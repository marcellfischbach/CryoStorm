
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/defs.hh>

namespace ce
{


class CS_CORE_API BoxClipper : public iClipper
{
public:
  explicit BoxClipper(const BoundingBox &bbox);
  BoxClipper(const Vector3f &min, const Vector3f max);
  ~BoxClipper() override = default;

  CS_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  Vector3f m_min;
  Vector3f m_max;
};


}