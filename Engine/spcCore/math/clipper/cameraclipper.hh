#pragma once


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

class SPC_CORE_API CameraClipper : public iClipper
{
public:
  CameraClipper(const Camera &camera, const Projector &projector, bool near = true, bool far = true);
  CameraClipper(const Matrix4f &camera, const Matrix4f &projector, bool near = true, bool far = true);
  ~CameraClipper() override = default;

  SPC_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  SPC_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  SPC_NODISCARD eClippingResult Test(const Vector3f& p) const override;



private:

  SPC_NODISCARD eClippingResult TestSimple(const BoundingBox &bbox) const;
  SPC_NODISCARD eClippingResult TestWithNear(const BoundingBox &bbox) const;
  SPC_NODISCARD eClippingResult TestWithFar(const BoundingBox &bbox) const;
  SPC_NODISCARD eClippingResult TestAll(const BoundingBox &bbox) const;
  Plane m_planes[6];
  enum class TestMode {
    Simple,
    All,
    WithNear,
    WithFar,
  };
  TestMode m_mode;
};

}

