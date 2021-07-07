

#include <spcCore/entity/camerastate.hh>
#include <spcCore/math/matrix4f.hh>
#include <spcCore/math/vector3f.hh>
#include <spcCore/math/vector4f.hh>
#include <math.h>


namespace spc
{



CameraState::CameraState()
  : SpatialState()
  , m_near(1.0f)
  , m_far (1024.0f)
  , m_angle ((float)M_PI / 4.0f)
  , m_angleWidthHeight(0.0f)
{
  SPC_CLASS_GEN_CONSTR;
}

void CameraState::SetNear(float near)
{
  m_near = near;
}

float CameraState::GetNear() const
{
  return m_near;
}

void CameraState::SetFar(float far)
{
  m_far = far;
}

float CameraState::GetFar() const
{
  return m_far;
}

void CameraState::SetAngle(float angle)
{
  m_angle = angle;
}

float CameraState::GetAngle() const
{
  return m_angle;
}
void CameraState::SetAngleWidthHeight(float angleWidthHeight)
{
  m_angleWidthHeight = angleWidthHeight;
}

float CameraState::GetAngleWidthHeight() const
{
  return m_angleWidthHeight;
}

const Camera& CameraState::GetCamera() const
{
  return m_camera;
}

const Projector& CameraState::GetProjector() const
{
  return m_projector;
}


void CameraState::Update(u32 width, u32 height)
{
  if (width == 0 || height == 0)
  {
    return;
  }

  const Matrix4f& m = GetGlobalMatrix();

  Vector3f eye(m * Vector4f(0.0, 0.0, 0.0, 1.0f));
  Vector3f dir(m * Vector4f(0.0, 0.0, 1.0, 0.0f));
  Vector3f up(m * Vector4f(0.0, 1.0, 0.0, 0.0f));

  m_camera.SetEye(eye);
  m_camera.SetSpot(eye + dir);
  m_camera.SetUp(up);


  float tan = spcTan(m_angle);
  float aspect = (float)height / (float)width;


  float rh = m_near * tan;
  float lh = -rh;
  float th = rh * aspect;
  float bh = -th;

  float tv = m_near * tan;
  float bv = -th;
  float rv = tv / aspect;
  float lv = -rv;


  float horizontal = 1.0f - m_angleWidthHeight;
  float vertical = m_angleWidthHeight;
  m_projector.UpdatePerspective(
    lh * horizontal + lv * vertical,
    rh * horizontal + rv * vertical,
    bh * horizontal + bv * vertical,
    th * horizontal + tv * vertical,
    m_near,
    m_far
  );

}

}