
#include <ceCore/graphics/csProjector.hh>
#include <ceCore/graphics/iDevice.hh>

namespace cryo
{

csProjector::csProjector()
  : m_mode(ePM_Perspective), m_left(-1.0f), m_right(1.0f), m_bottom(-1.0f), m_top(1.0f), m_near(1.0f), m_far(1024.0)
{
  CS_CLASS_GEN_CONSTR;
}

void csProjector::UpdatePerspective(float angleRad, float aspect, float near, float far)
{
  m_mode = ePM_Perspective;

  float halfAngle = angleRad / 2.0f;
  m_right = near * ceTan(halfAngle);
  m_left  = -m_right;

  m_top    = m_right * aspect;
  m_bottom = -m_top;

  m_near = near;
  m_far  = far;

}

void csProjector::UpdatePerspective(float left, float right, float bottom, float top, float near, float far)
{
  m_mode   = ePM_Perspective;
  m_left   = left;
  m_right  = right;
  m_bottom = bottom;
  m_top    = top;
  m_near   = near;
  m_far    = far;
}

void csProjector::UpdateOrtho(float left, float right, float bottom, float top, float near, float far)
{
  m_mode   = ePM_Orthographic;
  m_left   = left;
  m_right  = right;
  m_bottom = bottom;
  m_top    = top;
  m_near   = near;
  m_far    = far;
}

void csProjector::Bind(iDevice* device) const
{

  csMatrix4f projection, projectionInv;
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

csMatrix4f csProjector::GetProjectionMatrix(iDevice* device) const
{

  csMatrix4f matrix;
  switch (m_mode)
  {
  case ePM_Perspective:
    device->GetPerspectiveProjection(m_left, m_right, m_bottom, m_top, m_near, m_far, matrix);
    break;
  case ePM_Orthographic:
    device->GetOrthographicProjection(m_left, m_right, m_bottom, m_top, m_near, m_far, matrix);
    break;
  }

  return matrix;
}


csMatrix4f csProjector::GetProjectionMatrixInv(iDevice* device) const
{

  csMatrix4f matrix;
  switch (m_mode)
  {
  case ePM_Perspective:
    device->GetPerspectiveProjectionInv(m_left, m_right, m_bottom, m_top, m_near, m_far, matrix);
    break;
  case ePM_Orthographic:
    device->GetOrthographicProjectionInv(m_left, m_right, m_bottom, m_top, m_near, m_far, matrix);
    break;
  }

  return matrix;
}


void csProjector::GetPoints(float depth, csVector3f* out) const
{
  float depth_factor = 1.0f;
  switch (m_mode)
  {
  case ePM_Perspective:
    depth_factor = depth / m_near;
    break;
  case ePM_Orthographic:
    depth_factor = 1.0f;
    break;
  }

  float l = m_left * depth_factor;
  float r = m_right * depth_factor;
  float b = m_bottom * depth_factor;
  float t = m_top * depth_factor;

  out[0] = csVector3f(l, b, depth);
  out[1] = csVector3f(l, t, depth);
  out[2] = csVector3f(r, b, depth);
  out[3] = csVector3f(r, t, depth);

}

float csProjector::GetNear() const
{
  return m_near;
}

float csProjector::GetFar() const
{
  return m_far;
}

float csProjector::GetLeft() const
{
  return m_left;
}

float csProjector::GetRight() const
{
  return m_right;
}

float csProjector::GetBottom() const
{
  return m_bottom;
}

float csProjector::GetTop() const
{
  return m_top;
}

}