
#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/math/iclipper.hh>
#include <csCore/csDefs.hh>
#include <csCore/math/csPlane.hh>

namespace cryo
{


class CS_CORE_API csPlaneClipper : public iClipper
{
public:
  explicit csPlaneClipper(const csPlane &plane);
  ~csPlaneClipper() override = default;

  CS_NODISCARD eClippingResult Test(const csBoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const csSphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const csVector3f& p) const override;

private:

  csPlane m_plane;
};


}