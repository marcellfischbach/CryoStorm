
#pragma  once

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

class CS_CORE_API csMultiPlaneClipper : public iClipper
{
public:
  csMultiPlaneClipper();
  csMultiPlaneClipper(const csCamera &camera, const csProjector &projector, bool near = true, bool far = true);
  csMultiPlaneClipper(const csMatrix4f &camera, const csMatrix4f &projector, bool near = true, bool far = true);
  ~csMultiPlaneClipper() override = default;

  csMultiPlaneClipper& operator <<(const csPlane& plane);

  CS_NODISCARD eClippingResult Test(const csBoundingBox& bbox) const override;
  CS_NODISCARD eClippingResult Test(const csSphere& sphere) const override;
  CS_NODISCARD eClippingResult Test(const csVector3f& p) const override;

private:

  std::vector<csPlane> m_planes;
};


}