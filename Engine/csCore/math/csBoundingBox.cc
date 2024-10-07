

#include <csCore/math/csBoundingBox.hh>
#include <cfloat>

namespace cs {


csBoundingBox::csBoundingBox()
  : m_valid(false)
  , m_min(FLT_MAX, FLT_MAX, FLT_MAX)
  , m_max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
  , m_diagonal(-FLT_MAX)
{

}


void csBoundingBox::Add(const csVector3f &p)
{
  if (p.x > m_max.x) m_max.x = p.x;
  if (p.y > m_max.y) m_max.y = p.y;
  if (p.z > m_max.z) m_max.z = p.z;
  if (p.x < m_min.x) m_min.x = p.x;
  if (p.y < m_min.y) m_min.y = p.y;
  if (p.z < m_min.z) m_min.z = p.z;
}

void csBoundingBox::Add(const csBoundingBox &bbox)
{
  if (bbox.m_max.x > m_max.x) m_max.x = bbox.m_max.x;
  if (bbox.m_max.y > m_max.y) m_max.y = bbox.m_max.y;
  if (bbox.m_max.z > m_max.z) m_max.z = bbox.m_max.z;
  if (bbox.m_min.x < m_min.x) m_min.x = bbox.m_min.x;
  if (bbox.m_min.y < m_min.y) m_min.y = bbox.m_min.y;
  if (bbox.m_min.z < m_min.z) m_min.z = bbox.m_min.z;
}


void csBoundingBox::Add(const csMatrix4f &M, const csVector3f &p)
{
  csVector3f mp = csMatrix4f::Transform(M, p);
  if (mp.x > m_max.x) m_max.x = mp.x;
  if (mp.y > m_max.y) m_max.y = mp.y;
  if (mp.z > m_max.z) m_max.z = mp.z;
  if (mp.x < m_min.x) m_min.x = mp.x;
  if (mp.y < m_min.y) m_min.y = mp.y;
  if (mp.z < m_min.z) m_min.z = mp.z;
}

void csBoundingBox::Add(const csMatrix4f &M, const csBoundingBox &bbox)
{
  for (auto m_point : bbox.m_points)
  {
    Add(M, m_point);
  }
}


void csBoundingBox::Clear()
{
  m_valid = false;
  m_min = csVector3f(FLT_MAX, FLT_MAX, FLT_MAX);
  m_max = csVector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

void csBoundingBox::Finish()
{
  m_center = (m_min + m_max) * 0.5f;

  m_points[0] = csVector3f(m_min.x, m_min.y, m_min.z);
  m_points[1] = csVector3f(m_min.x, m_min.y, m_max.z);
  m_points[2] = csVector3f(m_min.x, m_max.y, m_min.z);
  m_points[3] = csVector3f(m_min.x, m_max.y, m_max.z);
  m_points[4] = csVector3f(m_max.x, m_min.y, m_min.z);
  m_points[5] = csVector3f(m_max.x, m_min.y, m_max.z);
  m_points[6] = csVector3f(m_max.x, m_max.y, m_min.z);
  m_points[7] = csVector3f(m_max.x, m_max.y, m_max.z);

  m_valid = true;

  m_diagonal = (m_max - m_min).Length();
}


void csBoundingBox::Debug(const char *message) const
{
  printf("ceBoundingBox ");
  if (message)
  {
    printf("\"%s\" ", message);
  }
  if (m_valid)
  {
    printf("<%.2f %.2f %.2f>  <%.2f %.2f %.2f>  <%.2f %.2f %.2f>  -  <%.2f %.2f %.2f>\n",
           m_center.x,
           m_center.y,
           m_center.z,
           m_max.x - m_min.x,
           m_max.y - m_min.y,
           m_max.z - m_min.z,
           m_min.x,
           m_min.y,
           m_min.z,
           m_max.x,
           m_max.y,
           m_max.z);
  }
  else
  {
    printf("invalid\n");
  }
}
}
