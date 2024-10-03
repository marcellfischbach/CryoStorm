
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/defs.hh>
#include <ceCore/math/plane.hh>

namespace ce
{


class CS_CORE_API PlaneClipper : public iClipper
{
public:
  explicit PlaneClipper(const Plane &plane);
  ~PlaneClipper() override = default;

  CS_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  Plane m_plane;
};


}