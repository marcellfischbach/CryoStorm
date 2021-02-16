
#include <spcCore/math/clipper/planeclipper.hh>

namespace spc
{


PlaneClipper::PlaneClipper(const Plane& plane)
        : iClipper()
        , m_plane(plane)
{

}
eClippingResult PlaneClipper::Test(const BoundingBox& bbox) const
{
  const Vector3f* points = bbox.GetPoints();

  Size in = 0;
  Size out = 0;
  for (Size i = 0; i < 8; ++i)
  {
    eClippingResult res = Test(points[i]);
    switch (res)
    {
      case eClippingResult::eCR_Inside:
        in++;
        break;
      case eClippingResult::eCR_Outside:
        out++;
        break;
      default:
        break;
    }
    if (in > 0 && out > 0)
    {
      return eClippingResult::eCR_Intermediate;
    }
  }

  if (in == 8)
  {
    return eClippingResult::eCR_Inside;
  }
  else if (out == 8)
  {
    return eClippingResult::eCR_Outside;
  }
  return eClippingResult::eCR_Intermediate;
}

eClippingResult PlaneClipper::Test(const Sphere& sphere) const
{
  float distance = m_plane.Distance(sphere.center);
  if (distance - sphere.radius >= 0.0f)
  {
    return eClippingResult::eCR_Inside;
  }
  else if (distance + sphere.radius < 0.0f)
  {
    return eClippingResult::eCR_Outside;
  }

  return eClippingResult::eCR_Intermediate;
}

eClippingResult PlaneClipper::Test(const Vector3f& p) const
{
  return m_plane.Distance(p) >= 0.0f
         ? eClippingResult::eCR_Inside
         : eClippingResult::eCR_Outside;
}


}