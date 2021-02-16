
#include <spcCore/math/clipper/multiplaneclipper.hh>

namespace spc
{


MultiPlaneClipper::MultiPlaneClipper()
        : iClipper()
{
}

MultiPlaneClipper& MultiPlaneClipper::operator<<(const Plane& plane)
{
  m_planes.push_back(plane);
  return *this;
}

eClippingResult MultiPlaneClipper::Test(const BoundingBox& bbox) const
{
  const Vector3f* points = bbox.GetPoints();

  eClippingResult res = eClippingResult::eCR_Inside;
  for (auto plane : m_planes)
  {
    Size in = 0;
    Size out = 0;
    for (Size i = 0; i < 8; ++i)
    {
      if (plane.Distance(points[i]) < 0.0)
      {
        out++;
      }
      else
      {
        in++;
      }
      if (in > 0 && out > 0)
      {
        res = eClippingResult::eCR_Intermediate;
        break;
      }
    }

    if (out == 8)
    {
      return eClippingResult::eCR_Outside;
    }
  }
  return res;
}

eClippingResult MultiPlaneClipper::Test(const Sphere& sphere) const
{
  eClippingResult result = eClippingResult::eCR_Inside;
  for (auto  plane : m_planes)
  {
    float distance = plane.Distance(sphere.center);
    if (distance + sphere.radius < 0.0f)
    {
      return eClippingResult::eCR_Outside;
    }
    else if (distance - sphere.radius < 0.0f)
    {
      result = eClippingResult::eCR_Intermediate;
    }
  }
  return result;
}

eClippingResult MultiPlaneClipper::Test(const Vector3f& p) const
{
  for (auto plane : m_planes)
  {
    if (plane.Distance(p) < 0)
    {
      return eClippingResult::eCR_Outside;
    }
  }
  return eClippingResult::eCR_Inside;
}

}