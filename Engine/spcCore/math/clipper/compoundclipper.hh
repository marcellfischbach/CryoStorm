
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/math/iclipper.hh>
#include <spcCore/defs.hh>
#include <vector>

namespace spc
{


class SPC_CORE_API CompoundClipper : public iClipper
{
public:
  explicit CompoundClipper();
  ~CompoundClipper() override = default;

  void AddClipper(const iClipper* clipper);


  SPC_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  SPC_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  SPC_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  std::vector<const iClipper*> m_clippers;
};


}