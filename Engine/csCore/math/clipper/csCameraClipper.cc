
#include <csCore/math/clipper/csCameraClipper.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/csProjector.hh>

namespace cs
{


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
csCameraClipper::csCameraClipper(const csCamera &camera, const csProjector &projector, bool near, bool far)
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

  m_planes[0].Set(n[0], f[1], f[0]); // left
  m_planes[1].Set(n[3], n[2], f[3]); // right;
  m_planes[2].Set(n[2], n[0], f[2]); // bottom
  m_planes[3].Set(n[1], f[3], f[1]); // top
  if (near)
  {
    m_planes[4].Set(n[0], n[3], n[1]); // near
  }
  if (far)
  {
    m_planes[5].Set(f[0], f[1], f[3]); // far
  }

  if (near && far)
  {
    m_mode = TestMode::All;
  }
  else if (near)
  {
    m_mode = TestMode::WithNear;
  }
  else if (far)
  {
    m_mode = TestMode::WithFar;
  }
  else
  {
    m_mode = TestMode::Simple;
  }
}


csCameraClipper::csCameraClipper(const csMatrix4f &camera, const csMatrix4f &projector, bool near, bool far)
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


  m_planes[0].Set((csVector3f) n0, (csVector3f) f1, (csVector3f) f0); // left
  m_planes[1].Set((csVector3f) n3, (csVector3f) n2, (csVector3f) f3); // right;
  m_planes[2].Set((csVector3f) n2, (csVector3f) n0, (csVector3f) f2); // bottom
  m_planes[3].Set((csVector3f) n1, (csVector3f) f3, (csVector3f) f1); // top
  if (near)
  {
    m_planes[4].Set((csVector3f) n0, (csVector3f) n3, (csVector3f) n1); // near
  }
  if (far)
  {
    m_planes[5].Set((csVector3f) f0, (csVector3f) f1, (csVector3f) f3); // far
  }
  if (near && far)
  {
    m_mode = TestMode::All;
  }
  else if (near)
  {
    m_mode = TestMode::WithNear;
  }
  else if (far)
  {
    m_mode = TestMode::WithFar;
  }
  else
  {
    m_mode = TestMode::Simple;
  }
}

CS_FORCEINLINE eClippingResult test_plane(const csPlane &plane, const csVector3f *pos)
{
  size_t      out = 0;
  for (size_t i   = 0; i < 8; ++i)
  {
    out += plane.Distance(pos[i]) < 0.0;
  }

  if (out == 8)
  {
    return eClippingResult::eCR_Outside;
  }
  else if (out == 0)
  {
    return eClippingResult::eCR_Inside;
  }
  return eClippingResult::eCR_Intermediate;
}

#define TEST_PLANE(i) \
  switch (test_plane(m_planes[i], points))  {         \
  case eClippingResult::eCR_Outside:                  \
    return eClippingResult::eCR_Outside;              \
  case eClippingResult::eCR_Intermediate:             \
    result = eClippingResult::eCR_Intermediate;       \
    break;                                            \
  case eClippingResult::eCR_Inside:                   \
    break;                                            \
}

eClippingResult csCameraClipper::TestSimple(const csBoundingBox &bbox) const
{
  const csVector3f *points = bbox.GetPoints();
  eClippingResult  result  = eClippingResult::eCR_Inside;
  TEST_PLANE(0)
  TEST_PLANE(1)
  TEST_PLANE(2)
  TEST_PLANE(3)
  return result;
}

eClippingResult csCameraClipper::TestWithNear(const csBoundingBox &bbox) const
{
  const csVector3f *points = bbox.GetPoints();
  eClippingResult  result  = eClippingResult::eCR_Inside;
  TEST_PLANE(0)
  TEST_PLANE(1)
  TEST_PLANE(2)
  TEST_PLANE(3)
  TEST_PLANE(4)
  return result;
}

eClippingResult csCameraClipper::TestWithFar(const csBoundingBox &bbox) const
{
  const csVector3f *points = bbox.GetPoints();
  eClippingResult  result  = eClippingResult::eCR_Inside;
  TEST_PLANE(0)
  TEST_PLANE(1)
  TEST_PLANE(2)
  TEST_PLANE(3)
  TEST_PLANE(5)
  return result;
}


eClippingResult csCameraClipper::TestAll(const csBoundingBox &bbox) const
{
  const csVector3f *points = bbox.GetPoints();
  eClippingResult  result  = eClippingResult::eCR_Inside;
  TEST_PLANE(0)
  TEST_PLANE(1)
  TEST_PLANE(2)
  TEST_PLANE(3)
  TEST_PLANE(4)
  TEST_PLANE(5)
  return result;
}


eClippingResult csCameraClipper::Test(const csBoundingBox &bbox) const
{
  switch (m_mode)
  {
  case TestMode::Simple:
    return TestSimple(bbox);
  case TestMode::WithFar:
    return TestWithFar(bbox);
  case TestMode::WithNear:
    return TestWithNear(bbox);
  case TestMode::All:
    return TestAll(bbox);
  }
  return eClippingResult::eCR_Inside;
}

eClippingResult csCameraClipper::Test(const csSphere &sphere) const
{
  eClippingResult result = eClippingResult::eCR_Inside;
  for (auto       plane: m_planes)
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

eClippingResult csCameraClipper::Test(const csVector3f &p) const
{
  for (auto plane: m_planes)
  {
    if (plane.Distance(p) < 0)
    {
      return eClippingResult::eCR_Outside;
    }
  }
  return eClippingResult::eCR_Inside;
}

}