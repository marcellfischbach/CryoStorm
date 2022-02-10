
#include <ceCore/math/clipper/sphereclipper.hh>

namespace ce
{


SphereClipper::SphereClipper(const Vector3f& center, float radius)
        : iClipper()
        , m_center(center)
        , m_radius(radius)
        , m_radiusSqr(radius * radius)
{

}
eClippingResult SphereClipper::Test(const BoundingBox& bbox) const
{
  const Vector3f& bboxMin = bbox.GetMin();
  const Vector3f& bboxMax = bbox.GetMax();

  const Vector3f min = Vector3f(m_center.x - m_radius, m_center.y - m_radius, m_center.z - m_radius);
  const Vector3f max = Vector3f(m_center.x + m_radius, m_center.y + m_radius, m_center.z + m_radius);

  if (max.x < bboxMin.x || max.y < bboxMin.y || max.z < bboxMin.z
      || min.x > bboxMax.x || min.y > bboxMax.y || min.z > bboxMax.z)
  {
    return eClippingResult::eCR_Outside;
  }

  const Vector3f* points = bbox.GetPoints();
  for (Size i = 0; i < 8; ++i)
  {
    eClippingResult res = Test(points[i]);
    if (res == eClippingResult::eCR_Outside)
    {
      // we are not completely outside so finding one point outside the sphere meaning the box is intermediate
      return eClippingResult::eCR_Intermediate;
    }
  }
  return eClippingResult::eCR_Inside;
}


eClippingResult SphereClipper::Test(const Sphere& sphere) const
{
  // TODO: Check if a calculation is possible without the SQRT
  float distance = (m_center - sphere.center).Length();
  if (distance > m_radius + sphere.radius)
  {
    return eClippingResult::eCR_Outside;
  }
  else if (distance + sphere.radius <= m_radius)
  {
    return eClippingResult::eCR_Inside;
  }
  return eClippingResult::eCR_Intermediate;
}


eClippingResult SphereClipper::Test(const Vector3f& p) const
{
  float d = (p - m_center).Dot();
  return d <= m_radiusSqr
         ? eClippingResult::eCR_Inside
         : eClippingResult::eCR_Outside;
}

}