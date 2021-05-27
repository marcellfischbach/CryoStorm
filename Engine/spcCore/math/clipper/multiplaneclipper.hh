
#pragma  once

#include <spcCore/coreexport.hh>
#include <spcCore/math/iclipper.hh>
#include <spcCore/defs.hh>
#include <spcCore/math/plane.hh>
#include <vector>

namespace spc
{

class Camera;
class Projector;
struct Matrix4f;

class SPC_CORE_API MultiPlaneClipper : public iClipper
{
public:
  MultiPlaneClipper();
  MultiPlaneClipper(const Camera &camera, const Projector &projector, bool near = true, bool far = true);
  MultiPlaneClipper(const Matrix4f &camera, const Matrix4f &projector, bool near = true, bool far = true);
  ~MultiPlaneClipper() override = default;

  MultiPlaneClipper& operator <<(const Plane& plane);

  SPC_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  SPC_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  SPC_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  std::vector<Plane> m_planes;
};


}