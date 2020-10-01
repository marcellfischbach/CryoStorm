
#include <spcCore/math/planeclipper.hh>


namespace spc {


PlaneClipper::PlaneClipper()
{

}

PlaneClipper::~PlaneClipper()
{
  m_planes.clear();
}

void PlaneClipper::Clear()
{
  m_planes.clear();
}

void PlaneClipper::AddPlane(const Plane &plane)
{
  m_planes.push_back(plane);
}

Clipper::ClipResult PlaneClipper::Test(const Vector3f &p) const
{
  for (size_t i = 0, in = m_planes.size(); i < in; ++i)
  {
    const Plane &plane = m_planes[i];
    if (plane.Distance(p) < 0.0f)
    {
      return eCR_Out;
    }
  }

  return eCR_In;
}


Clipper::ClipResult PlaneClipper::Test(const BoundingBox &bbox, bool debug) const
{
  if (!bbox.IsValid())
  {
    return eCR_Intermediate;
  }

  ClipResult res = eCR_In;
  if (debug) printf("    Clipper::Test\n");
  const Vector3f *points = bbox.GetPoints();
  for (size_t i = 0, in = m_planes.size(); i < in; ++i)
  {
    const Plane &plane = m_planes[i];
    if (debug) printf("      <%.2f %.2f %.2f %.2f>\n", plane.x, plane.y, plane.z, plane.d);
    int out = 0;
    for (size_t j = 0; j < 8; ++j)
    {
      float dist = plane.Distance(points[j]);
      if (debug) printf("        <%.2f %.2f %.2f> => %.2f\n", points[j].x, points[j].y, points[j].z, dist);
      if (dist < 0.0f)
      {
        out++;
      }
    }

    if (out == 8)
    {
      if (debug) printf("      out\n");
      return eCR_Out;
    }
    if (out > 0)
    {
      res = eCR_Intermediate;
    }
  }

  return res;
}

}
