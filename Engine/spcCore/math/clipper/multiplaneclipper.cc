
#include <spcCore/math/clipper/multiplaneclipper.hh>
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/projector.hh>

namespace spc
{


MultiPlaneClipper::MultiPlaneClipper()
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
MultiPlaneClipper::MultiPlaneClipper(const Camera &camera, const Projector &projector, bool near, bool far)
    : iClipper()
{
  Matrix4f viewMatrixInv;
  viewMatrixInv.SetLookAtInv(camera.GetEye(), camera.GetSpot(), camera.GetUp());

  Vector3f n[4];
  Vector3f f[4];
  projector.GetPoints(projector.GetNear(), n);
  projector.GetPoints(projector.GetFar(), f);

  for (size_t i = 0; i < 4; i++)
  {
    n[i] = Matrix4f::Transform(viewMatrixInv, n[i]);
    f[i] = Matrix4f::Transform(viewMatrixInv, f[i]);
  }

  m_planes.emplace_back(Plane(n[0], f[1], f[0])); // left
  m_planes.emplace_back(Plane(n[3], n[2], f[3])); // right;
  m_planes.emplace_back(Plane(n[2], n[0], f[2])); // bottom
  m_planes.emplace_back(Plane(n[1], f[3], f[1])); // top
  if (near)
  {
    m_planes.emplace_back(Plane(n[0], n[3], n[1])); // near
  }
  if (far)
  {
    m_planes.emplace_back(Plane(f[0], f[1], f[3])); // far
  }
}


MultiPlaneClipper::MultiPlaneClipper(const Matrix4f &camera, const Matrix4f &projector, bool near, bool far)
    : iClipper()
{

  Matrix4f M = camera * projector;

  // TODO: Reduce multiplications of not needed position
  Vector4f n0 = M * Vector4f(-1.0f, -1.0f, -1.0f, 1.0f);
  Vector4f n1 = M * Vector4f(-1.0f, 1.0f, -1.0f, 1.0f);
  Vector4f n2 = M * Vector4f(1.0f, -1.0f, -1.0f, 1.0f);
  Vector4f n3 = M * Vector4f(1.0f, 1.0f, -1.0f, 1.0f);
  Vector4f f0 = M * Vector4f(-1.0f, -1.0f, 1.0f, 1.0f);
  Vector4f f1 = M * Vector4f(-1.0f, 1.0f, 1.0f, 1.0f);
  Vector4f f2 = M * Vector4f(1.0f, -1.0f, 1.0f, 1.0f);
  Vector4f f3 = M * Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
  n0 /= n0.w;
  n1 /= n1.w;
  n2 /= n2.w;
  n3 /= n3.w;
  f0 /= f0.w;
  f1 /= f1.w;
  f2 /= f2.w;
  f3 /= f3.w;


  m_planes.emplace_back(Plane((Vector3f)n0, (Vector3f)f1, (Vector3f)f0)); // left
  m_planes.emplace_back(Plane((Vector3f)n3, (Vector3f)n2, (Vector3f)f3)); // right;
  m_planes.emplace_back(Plane((Vector3f)n2, (Vector3f)n0, (Vector3f)f2)); // bottom
  m_planes.emplace_back(Plane((Vector3f)n1, (Vector3f)f3, (Vector3f)f1)); // top
  if (near)
  {
    m_planes.emplace_back(Plane((Vector3f)n0, (Vector3f)n3, (Vector3f)n1)); // near
  }
  if (far)
  {
    m_planes.emplace_back(Plane((Vector3f)f0, (Vector3f)f1, (Vector3f)f3)); // far
  }
}


MultiPlaneClipper &MultiPlaneClipper::operator<<(const Plane &plane)
{
  m_planes.push_back(plane);
  return *this;
}

eClippingResult MultiPlaneClipper::Test(const BoundingBox &bbox) const
{
  const Vector3f *points = bbox.GetPoints();

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

eClippingResult MultiPlaneClipper::Test(const Sphere &sphere) const
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

eClippingResult MultiPlaneClipper::Test(const Vector3f &p) const
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