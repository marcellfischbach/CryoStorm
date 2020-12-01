
#include <spcCore/graphics/projector.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/math/math.hh>

namespace spc
{

Projector::Projector()
  : m_mode(ePM_Perspective)
  , m_left(-1.0f)
  , m_right(1.0f)
  , m_bottom(-1.0f)
  , m_top(1.0f)
  , m_near(1.0f)
  , m_far(1024.0)
{
}

  
void Projector::UpdatePerspective(float angleRad, float aspect, float near, float far)
{
  m_mode = ePM_Perspective;

  float halfAngle = angleRad / 2.0f;
  m_right = spcTan(halfAngle);
  m_left = -m_right;

  m_top = m_right * aspect;
  m_bottom = -m_top;
  
  m_near = near;
  m_far = far;
}

void Projector::Bind(iDevice* device)
{

  Matrix4f projection, projectionInv;
  switch (m_mode)
  {
  case ePM_Perspective:
    device->GetPerspectiveProjection(m_left, m_right, m_bottom, m_top, m_near, m_far, projection);
    device->GetPerspectiveProjectionInv(m_left, m_right, m_bottom, m_top, m_near, m_far, projectionInv);
    break;
  case ePM_Orthographic:
    device->GetOrthographicProjection(m_left, m_right, m_bottom, m_top, m_near, m_far, projection);
    device->GetOrthographicProjectionInv(m_left, m_right, m_bottom, m_top, m_near, m_far, projectionInv);
    break;
  }

  device->SetProjectionMatrix(projection, projectionInv);

}

}