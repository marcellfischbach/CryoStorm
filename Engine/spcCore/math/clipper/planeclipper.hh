
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/math/iclipper.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/plane.hh>

namespace spc
{


class SPC_CORE_API PlaneClipper : public iClipper
{
public:
  explicit PlaneClipper(const Plane &plane);
  ~PlaneClipper() override = default;

  SPC_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  SPC_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  SPC_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  Plane m_plane;
};


}