
#include <cecore/math/ceplaneclipper.hh>

cePlaneClipper::cePlaneClipper()
{

}

cePlaneClipper::~cePlaneClipper()
{
  m_planes.clear();
}

void cePlaneClipper::Clear()
{
  m_planes.clear();
}

void cePlaneClipper::AddPlane(const cePlane &plane)
{
  m_planes.push_back(plane);
}

ceClipper::ClipResult cePlaneClipper::Test(const ceVector3f &p) const
{
  for (size_t i = 0, in = m_planes.size(); i < in; ++i)
  {
    const cePlane &plane = m_planes[i];
    if (plane.Distance(p) < 0.0f)
    {
      return eCR_Out;
    }
  }

  return eCR_In;
}


ceClipper::ClipResult cePlaneClipper::Test(const ceBoundingBox &bbox, bool debug) const
{
  if (!bbox.IsValid())
  {
    return eCR_Intermediate;
  }

  ClipResult res = eCR_In;
  if (debug) printf("    Clipper::Test\n");
  const ceVector3f *points = bbox.GetPoints();
  for (size_t i = 0, in = m_planes.size(); i < in; ++i)
  {
    const cePlane &plane = m_planes[i];
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
