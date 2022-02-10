#pragma once


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

class CE_CORE_API CameraClipper : public iClipper
{
public:
  CameraClipper(const Camera &camera, const Projector &projector, bool near = true, bool far = true);
  CameraClipper(const Matrix4f &camera, const Matrix4f &projector, bool near = true, bool far = true);
  ~CameraClipper() override = default;

  CE_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  CE_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  CE_NODISCARD eClippingResult Test(const Vector3f& p) const override;



private:

  CE_NODISCARD eClippingResult TestSimple(const BoundingBox &bbox) const;
  CE_NODISCARD eClippingResult TestWithNear(const BoundingBox &bbox) const;
  CE_NODISCARD eClippingResult TestWithFar(const BoundingBox &bbox) const;
  CE_NODISCARD eClippingResult TestAll(const BoundingBox &bbox) const;
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

