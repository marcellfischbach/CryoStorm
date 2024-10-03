
#pragma  once

#include <ceCore/coreexport.hh>
#include <ceCore/math/iclipper.hh>
#include <ceCore/defs.hh>
#include <ceCore/math/plane.hh>
#include <vector>

namespace ce
{

class Camera;
class Projector;
struct Matrix4f;

class CS_CORE_API MultiPlaneClipper : public iClipper
{
public:
  MultiPlaneClipper();
  MultiPlaneClipper(const Camera &camera, const Projector &projector, bool near = true, bool far = true);
  MultiPlaneClipper(const Matrix4f &camera, const Matrix4f &projector, bool near = true, bool far = true);
  ~MultiPlaneClipper() override = default;

  MultiPlaneClipper& operator <<(const Plane& plane);

  CS_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const Vector3f& p) const override;

private:

  std::vector<Plane> m_planes;
};


}