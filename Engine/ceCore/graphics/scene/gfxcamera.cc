
#include <ceCore/graphics/scene/gfxcamera.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/postprocessing.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/irendertarget2d.hh>


namespace ce
{


GfxCamera::GfxCamera()
  : m_camera(nullptr), m_projector(nullptr), m_near(0.0f), m_far(1024.0f), m_angle(0.0f), m_angleWidthHeight(0.0f)
  , m_renderTarget(nullptr)
  , m_order(0)
  , m_renderShadows(true)
  , m_postProcessing(nullptr)
{
  CE_CLASS_GEN_CONSTR;
}

GfxCamera::~GfxCamera()
{
  CE_RELEASE(m_camera);
  CE_RELEASE(m_projector);
}


void GfxCamera::SetCamera(Camera* camera)
{
  CE_SET(m_camera, camera);
}

Camera* GfxCamera::GetCamera()
{
  return m_camera;
}

const Camera* GfxCamera::GetCamera() const
{
  return m_camera;
}

void GfxCamera::SetProjector(Projector* projector)
{
  CE_SET(m_projector, projector);
}

Projector* GfxCamera::GetProjector()
{
  return m_projector;
}

const Projector* GfxCamera::GetProjector() const
{
  return m_projector;
}

eClearMode GfxCamera::GetClearMode() const
{
  return m_clearMode;
}

const Color4f& GfxCamera::GetClearColor() const
{
  return m_clearColor;
}

float GfxCamera::GetClearDepth() const
{
  return m_clearDepth;
}

void GfxCamera::SetRenderTarget(iRenderTarget2D* renderTarget)
{
  CE_SET(m_renderTarget, renderTarget);
}

iRenderTarget2D* GfxCamera::GetRenderTarget()
{
  return m_renderTarget;
}

const iRenderTarget2D* GfxCamera::GetRenderTarget() const
{
  return m_renderTarget;
}



void GfxCamera::SetOrder(int order)
{
  m_order = order;
}

int GfxCamera::GetOrder() const
{
  return m_order;
}

void GfxCamera::SetRenderShadows(bool renderShadows)
{
  m_renderShadows = renderShadows;
}

bool GfxCamera::IsRenderShadows() const
{
  return m_renderShadows;
}

void GfxCamera::SetPostProcessing(PostProcessing* postProcessing)
{
  CE_SET(m_postProcessing, postProcessing);
}

PostProcessing* GfxCamera::GetPostProcessing()
{
  return m_postProcessing;
}

const PostProcessing* GfxCamera::GetPostProcessing() const
{
  return m_postProcessing;
}


void GfxCamera::UpdateData(float near, float far, float angle, float angleWidthHeight)
{
  m_near = near;
  m_far = far;
  m_angle = angle;
  m_angleWidthHeight = angleWidthHeight;
}

void GfxCamera::UpdateClear(eClearMode mode, const Color4f& color, float depth)
{
  m_clearMode = mode;
  m_clearColor = color;
  m_clearDepth = depth;
}

void GfxCamera::UpdateProjector(uint32_t width, uint32_t height)
{
  float tan = ceTan(m_angle);
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