
#pragma  once

#include <csCore/csCoreExport.hh>
#include <csCore/math/iclipper.hh>
#include <csCore/csDefs.hh>
#include <vector>

namespace cs
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