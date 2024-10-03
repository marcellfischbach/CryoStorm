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

class CS_CORE_API CameraClipper : public iClipper
{
public:
  CameraClipper(const Camera &camera, const Projector &projector, bool near = true, bool far = true);
  CameraClipper(const Matrix4f &camera, const Matrix4f &projector, bool near = true, bool far = true);
  ~CameraClipper() override = default;

  CS_NODISCARD eClippingResult Test(const BoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const Sphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const Vector3f& p) const override;



private:

  CS_NODISCARD eClippingResult TestSimple(const BoundingBox &bbox) const;
  CS_NODISCARD eClippingResult TestWithNear(const BoundingBox &bbox) const;
  CS_NODISCARD eClippingResult TestWithFar(const BoundingBox &bbox) const;
  CS_NODISCARD eClippingResult TestAll(const BoundingBox &bbox) const;
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

