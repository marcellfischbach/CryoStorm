
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/defs.hh>
#include <vector>

namespace cryo
{


class CS_CORE_API CompoundClipper : public iClipper
{
public:
  explicit CompoundClipper();
  ~CompoundClipper() override = default;

  void AddClipper(const iClipper* clipper);


  CS_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  std::vector<const iClipper*> m_clippers;
};


}