
#include <csCore/math/clipper/csMultiPlaneClipper.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/csProjector.hh>

namespace cryo
{


csMultiPlaneClipper::csMultiPlaneClipper()
    : iClipper()
{
}


/*
 *
 *    1             3
 *     +----------+
 *     |          |
 *     |          |
 *     |          |
 *     +----------+
 *    0             2
 */
csMultiPlaneClipper::csMultiPlaneClipper(const csCamera &camera, const csProjector &projector, bool near, bool far)
    : iClipper()
{
  csMatrix4f viewMatrixInv;
  viewMatrixInv.SetLookAtInv(camera.GetEye(), camera.GetSpot(), camera.GetUp());

  csVector3f n[4];
  csVector3f f[4];
  projector.GetPoints(projector.GetNear(), n);
  projector.GetPoints(projector.GetFar(), f);

  for (size_t i = 0; i < 4; i++)
  {
    n[i] = csMatrix4f::Transform(viewMatrixInv, n[i]);
    f[i] = csMatrix4f::Transform(viewMatrixInv, f[i]);
  }

  m_planes.emplace_back(csPlane(n[0], f[1], f[0])); // left
  m_planes.emplace_back(csPlane(n[3], n[2], f[3])); // right;
  m_planes.emplace_back(csPlane(n[2], n[0], f[2])); // bottom
  m_planes.emplace_back(csPlane(n[1], f[3], f[1])); // top
  if (near)
  {
    m_planes.emplace_back(csPlane(n[0], n[3], n[1])); // near
  }
  if (far)
  {
    m_planes.emplace_back(csPlane(f[0], f[1], f[3])); // far
  }
}


csMultiPlaneClipper::csMultiPlaneClipper(const csMatrix4f &camera, const csMatrix4f &projector, bool near, bool far)
    : iClipper()
{

  csMatrix4f M = camera * projector;

  // TODO: Reduce multiplications of not needed position
  csVector4f n0 = M * csVector4f(-1.0f, -1.0f, -1.0f, 1.0f);
  csVector4f n1 = M * csVector4f(-1.0f, 1.0f, -1.0f, 1.0f);
  csVector4f n2 = M * csVector4f(1.0f, -1.0f, -1.0f, 1.0f);
  csVector4f n3 = M * csVector4f(1.0f, 1.0f, -1.0f, 1.0f);
  csVector4f f0 = M * csVector4f(-1.0f, -1.0f, 1.0f, 1.0f);
  csVector4f f1 = M * csVector4f(-1.0f, 1.0f, 1.0f, 1.0f);
  csVector4f f2 = M * csVector4f(1.0f, -1.0f, 1.0f, 1.0f);
  csVector4f f3 = M * csVector4f(1.0f, 1.0f, 1.0f, 1.0f);
  n0 /= n0.w;
  n1 /= n1.w;
  n2 /= n2.w;
  n3 /= n3.w;
  f0 /= f0.w;
  f1 /= f1.w;
  f2 /= f2.w;
  f3 /= f3.w;


  m_planes.emplace_back(csPlane((csVector3f)n0, (csVector3f)f1, (csVector3f)f0)); // left
  m_planes.emplace_back(csPlane((csVector3f)n3, (csVector3f)n2, (csVector3f)f3)); // right;
  m_planes.emplace_back(csPlane((csVector3f)n2, (csVector3f)n0, (csVector3f)f2)); // bottom
  m_planes.emplace_back(csPlane((csVector3f)n1, (csVector3f)f3, (csVector3f)f1)); // top
  if (near)
  {
    m_planes.emplace_back(csPlane((csVector3f)n0, (csVector3f)n3, (csVector3f)n1)); // near
  }
  if (far)
  {
    m_planes.emplace_back(csPlane((csVector3f)f0, (csVector3f)f1, (csVector3f)f3)); // far
  }
}


csMultiPlaneClipper &csMultiPlaneClipper::operator<<(const csPlane &plane)
{
  m_planes.push_back(plane);
  return *this;
}

eClippingResult csMultiPlaneClipper::Test(const csBoundingBox &bbox) const
{
  const csVector3f *points = bbox.GetPoints();

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

eClippingResult csMultiPlaneClipper::Test(const csSphere &sphere) const
{
  eClippingResult result = eClippingResult::eCR_Inside;
  for (auto plane : m_planes)
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

eClippingResult csMultiPlaneClipper::Test(const csVector3f &p) const
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