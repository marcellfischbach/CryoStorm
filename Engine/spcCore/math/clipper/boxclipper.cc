
#include <spcCore/math/clipper/boxclipper.hh>

namespace spc
{


BoxClipper::BoxClipper(const BoundingBox& bbox)
        : iClipper()
        , m_min(bbox.GetMin())
        , m_max(bbox.GetMax())
{

}

BoxClipper::BoxClipper(const Vector3f& min, const Vector3f max)
        : iClipper()
        , m_min(min)
        , m_max(max)
{

}
eClippingResult BoxClipper::Test(const BoundingBox& bbox) const
{
  const Vector3f& bboxMin = bbox.GetMin();
  const Vector3f& bboxMax = bbox.GetMax();

  if (m_max.x < bboxMin.x || m_max.y < bboxMin.y || m_max.z < bboxMin.z
      || m_min.x > bboxMax.x || m_min.y > bboxMax.y || m_min.z > bboxMax.z)
  {
    return eClippingResult::eCR_Outside;
  }
  else if (m_min.x <= bboxMin.x && m_min.y <= bboxMin.y && m_min.z <= bboxMin.z
           && m_max.x >= bboxMax.x && m_max.y >= bboxMax.y && m_max.z >= bboxMax.z)
  {
    return eClippingResult::eCR_Inside;
  }
  return eClippingResult::eCR_Intermediate;
}

eClippingResult BoxClipper::Test(const Sphere& sphere) const
{

  const Vector3f sphereMin = Vector3f(sphere.center.x - sphere.radius, sphere.center.y - sphere.radius,
                                      sphere.center.z - sphere.radius);
  const Vector3f sphereMax = Vector3f(sphere.center.x + sphere.radius, sphere.center.y + sphere.radius,
                                      sphere.center.z + sphere.radius);

  if (m_max.x < sphereMin.x || m_max.y < sphereMin.y || m_max.z < sphereMin.z
      || m_min.x > sphereMax.x || m_min.y > sphereMax.y || m_min.z > sphereMax.z)
  {
    return eClippingResult::eCR_Outside;
  }
  else if (m_min.x <= sphereMin.x && m_min.y <= sphereMin.y && m_min.z <= sphereMin.z
           && m_max.x >= sphereMax.x && m_max.y >= sphereMax.y && m_max.z <= sphereMax.z)
  {
    return eClippingResult::eCR_Inside;
  }


  return eClippingResult::eCR_Intermediate;
}


eClippingResult BoxClipper::Test(const Vector3f& p) const
{
  return m_min.x <= p.x && p.x <= m_max.x
         && m_min.y <= p.y && p.y <= m_max.y
         && m_min.z <= p.z && p.z <= m_max.z
         ? eClippingResult::eCR_Inside
         : eClippingResult::eCR_Outside;
}

}