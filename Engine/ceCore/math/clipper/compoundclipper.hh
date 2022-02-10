
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/defs.hh>
#include <vector>

namespace ce
{


class CE_CORE_API CompoundClipper : public iClipper
{
public:
  explicit CompoundClipper();
  ~CompoundClipper() override = default;

  void AddClipper(const iClipper* clipper);


  CE_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  CE_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  CE_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  std::vector<const iClipper*> m_clippers;
};


}