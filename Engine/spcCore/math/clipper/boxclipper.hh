
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/math/iclipper.hh>
#include <spcCore/defs.hh>

namespace spc
{


class SPC_CORE_API BoxClipper : public iClipper
{
public:
  explicit BoxClipper(const BoundingBox &bbox);
  BoxClipper(const Vector3f &min, const Vector3f max);
  ~BoxClipper() override = default;

  SPC_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  SPC_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  SPC_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  Vector3f m_min;
  Vector3f m_max;
};


}