
#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/math/iclipper.hh>
#include <csCore/csDefs.hh>

namespace cs
{


class CS_CORE_API csBoxClipper : public iClipper
{
public:
  explicit csBoxClipper(const csBoundingBox &bbox);
  csBoxClipper(const csVector3f &min, const csVector3f max);
  ~csBoxClipper() override = default;

  CS_NODISCARD eClippingResult Test(const csBoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const csSphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const csVector3f& p) const override;

private:

  csVector3f m_min;
  csVector3f m_max;
};


}