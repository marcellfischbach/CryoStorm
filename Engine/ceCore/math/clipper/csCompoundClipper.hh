
#pragma  once

#include <ceCore/csCoreExport.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/csDefs.hh>
#include <vector>

namespace cryo
{


class CS_CORE_API csCompoundClipper : public iClipper
{
public:
  explicit csCompoundClipper();
  ~csCompoundClipper() override = default;

  void AddClipper(const iClipper* clipper);


  CS_NODISCARD eClippingResult Test(const csBoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const csSphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const csVector3f& p) const override;

private:

  std::vector<const iClipper*> m_clippers;
};


}