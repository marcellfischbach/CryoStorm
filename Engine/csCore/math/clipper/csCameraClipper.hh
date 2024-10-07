#pragma once


#include <csCore/csCoreExport.hh>
#include <csCore/math/iclipper.hh>
#include <csCore/csDefs.hh>
#include <csCore/math/csPlane.hh>
#include <vector>

namespace cs
{

class csCamera;
class csProjector;
struct csMatrix4f;

class CS_CORE_API csCameraClipper : public iClipper
{
public:
  csCameraClipper(const csCamera &camera, const csProjector &projector, bool near = true, bool far = true);
  csCameraClipper(const csMatrix4f &camera, const csMatrix4f &projector, bool near = true, bool far = true);
  ~csCameraClipper() override = default;

  CS_NODISCARD eClippingResult Test(const csBoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const csSphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const csVector3f& p) const override;



private:

  CS_NODISCARD eClippingResult TestSimple(const csBoundingBox &bbox) const;
  CS_NODISCARD eClippingResult TestWithNear(const csBoundingBox &bbox) const;
  CS_NODISCARD eClippingResult TestWithFar(const csBoundingBox &bbox) const;
  CS_NODISCARD eClippingResult TestAll(const csBoundingBox &bbox) const;
  csPlane m_planes[6];
  enum class TestMode {
    Simple,
    All,
    WithNear,
    WithFar,
  };
  TestMode m_mode;
};

}

