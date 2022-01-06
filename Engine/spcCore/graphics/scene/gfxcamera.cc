
#include <spcCore/graphics/scene/gfxcamera.hh>
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/projector.hh>

namespace spc
{


GfxCamera::GfxCamera()
    : m_camera(nullptr)
    , m_projector(nullptr)
    , m_near(0.0f)
    , m_far(1024.0f)
    , m_angle(0.0f)
    , m_angleWidthHeight(0.0f)
{
  SPC_CLASS_GEN_CONSTR;
}

GfxCamera::~GfxCamera()
{
  SPC_RELEASE(m_camera);
  SPC_RELEASE(m_projector);
}


void GfxCamera::SetCamera(Camera *camera)
{
  SPC_SET(m_camera, camera);
}

Camera *GfxCamera::GetCamera()
{
  return m_camera;
}

const Camera *GfxCamera::GetCamera() const
{
  return m_camera;
}

void GfxCamera::SetProjector(Projector *projector)
{
  SPC_SET(m_projector, projector);
}

Projector *GfxCamera::GetProjector()
{
  return m_projector;
}

const Projector *GfxCamera::GetProjector() const
{
  return m_projector;
}

void GfxCamera::UpdateData(float near, float far, float angle, float angleWidthHeight)
{
  m_near             = near;
  m_far              = far;
  m_angle            = angle;
  m_angleWidthHeight = angleWidthHeight;
}

void GfxCamera::UpdateProjector(uint32_t width, uint32_t height)
{
  float tan    = spcTan(m_angle);
  float aspect = (float) height / (float) width;


  float rh = m_near * tan;
  float lh = -rh;
  float th = rh * aspect;
  float bh = -th;

  float tv = m_near * tan;
  float bv = -th;
  float rv = tv / aspect;
  float lv = -rv;


  float horizontal = 1.0f - m_angleWidthHeight;
  float vertical   = m_angleWidthHeight;
  m_projector->UpdatePerspective(
      lh * horizontal + lv * vertical,
      rh * horizontal + rv * vertical,
      bh * horizontal + bv * vertical,
      th * horizontal + tv * vertical,
      m_near,
      m_far
  );

}

}