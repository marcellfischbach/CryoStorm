
#include <ceCore/graphics/scene/csGfxCamera.hh>
#include <ceCore/graphics/csCamera.hh>
#include <ceCore/graphics/csPostProcessing.hh>
#include <ceCore/graphics/csProjector.hh>
#include <ceCore/graphics/iRenderTarget2D.hh>
#include <ceCore/graphics/iSkyboxRenderer.hh>
#include <ceCore/graphics/iTextureCube.hh>


namespace cryo
{


csGfxCamera::csGfxCamera()
    : m_camera(nullptr)
    , m_projector(nullptr)
    , m_near(0.0f)
    , m_far(1024.0f)
    , m_angle(0.0f)
    , m_angleWidthHeight(0.0f)
    , m_renderTarget(nullptr)
    , m_order(0)
    , m_skyboxRenderer(nullptr)
    , m_renderShadows(true)
    , m_postProcessing(nullptr)
{
  CS_CLASS_GEN_CONSTR;
}

csGfxCamera::~csGfxCamera()
{
  CS_RELEASE(m_camera);
  CS_RELEASE(m_projector);
}


void csGfxCamera::SetCamera(csCamera *camera)
{
  CS_SET(m_camera, camera);
}

csCamera *csGfxCamera::GetCamera()
{
  return m_camera;
}

const csCamera *csGfxCamera::GetCamera() const
{
  return m_camera;
}

void csGfxCamera::SetProjector(csProjector *projector)
{
  CS_SET(m_projector, projector);
}

csProjector *csGfxCamera::GetProjector()
{
  return m_projector;
}

const csProjector *csGfxCamera::GetProjector() const
{
  return m_projector;
}

eClearMode csGfxCamera::GetClearMode() const
{
  return m_clearMode;
}

eClearColorMode csGfxCamera::GetClearColorMode() const
{
  return m_clearColorMode;
}

const csColor4f &csGfxCamera::GetClearColor() const
{
  return m_clearColor;
}

float csGfxCamera::GetClearDepth() const
{
  return m_clearDepth;
}

void csGfxCamera::SetRenderTarget(iRenderTarget2D *renderTarget)
{
  CS_SET(m_renderTarget, renderTarget);
}

iRenderTarget2D *csGfxCamera::GetRenderTarget()
{
  return m_renderTarget;
}

void csGfxCamera::SetSkyboxRenderer(cryo::iSkyboxRenderer *skyboxRenderer)
{
  CS_SET(m_skyboxRenderer, skyboxRenderer);
}

iSkyboxRenderer *csGfxCamera::GetSkyboxRenderer() const
{
  return m_skyboxRenderer;
}

const iRenderTarget2D *csGfxCamera::GetRenderTarget() const
{
  return m_renderTarget;
}


void csGfxCamera::SetOrder(int order)
{
  m_order = order;
}

int csGfxCamera::GetOrder() const
{
  return m_order;
}

void csGfxCamera::SetRenderShadows(bool renderShadows)
{
  m_renderShadows = renderShadows;
}

bool csGfxCamera::IsRenderShadows() const
{
  return m_renderShadows;
}

void csGfxCamera::SetPostProcessing(csPostProcessing *postProcessing)
{
  CS_SET(m_postProcessing, postProcessing);
}

csPostProcessing *csGfxCamera::GetPostProcessing() const
{
  return m_postProcessing;
}


void csGfxCamera::UpdateData(float near, float far, float angle, float angleWidthHeight)
{
  m_near             = near;
  m_far              = far;
  m_angle            = angle;
  m_angleWidthHeight = angleWidthHeight;
}

void csGfxCamera::UpdateClear(eClearMode mode,
                              eClearColorMode clearColorMode,
                              const csColor4f &color,
                              iSkyboxRenderer *skybox,
                              float depth)
{
  m_clearMode      = mode;
  m_clearColorMode = clearColorMode;
  m_clearColor     = color;
  CS_SET(m_skyboxRenderer, skybox);
  m_clearDepth = depth;
}

void csGfxCamera::UpdateProjector(uint32_t width, uint32_t height)
{
  float tan    = ceTan(m_angle);
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