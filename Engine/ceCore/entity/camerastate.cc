

#include <ceCore/entity/camerastate.hh>
#include <ceCore/math/matrix4f.hh>
#include <ceCore/math/vector3f.hh>
#include <ceCore/math/vector4f.hh>
#include <math.h>
#include <ceCore/graphics/irendertarget2d.hh>
#include <ceCore/graphics/postprocessing.hh>
#include <ceCore/graphics/scene/gfxcamera.hh>
#include <ceCore/graphics/scene/igfxscene.hh>
#include <ceCore/entity/world.hh>

namespace ce
{


CameraState::CameraState()
    : SpatialState(),
      m_near(1.0f),
      m_far(1024.0f),
      m_angle((float) M_PI / 4.0f),
      m_angleWidthHeight(0.0f),
      m_gfxCamera(new GfxCamera()),
      m_order(0),
      m_renderTarget(nullptr),
      m_postProcessing(nullptr)
{
  CE_CLASS_GEN_CONSTR;
  m_gfxCamera->SetCamera(&m_camera);
  m_gfxCamera->SetProjector(&m_projector);
  UpdateGfxCamera();
}

CameraState::~CameraState()
{
  m_gfxCamera->Release();
}

void CameraState::SetNear(float near)
{
  m_near = near;
  UpdateGfxCamera();
}

float CameraState::GetNear() const
{
  return m_near;
}

void CameraState::SetFar(float far)
{
  m_far = far;
  UpdateGfxCamera();
}

float CameraState::GetFar() const
{
  return m_far;
}

void CameraState::SetAngle(float angle)
{
  m_angle = angle;
  UpdateGfxCamera();
}

float CameraState::GetAngle() const
{
  return m_angle;
}

void CameraState::SetAngleWidthHeight(float angleWidthHeight)
{
  m_angleWidthHeight = angleWidthHeight;
  UpdateGfxCamera();
}

float CameraState::GetAngleWidthHeight() const
{
  return m_angleWidthHeight;
}

void CameraState::SetOrder(int order)
{
  m_order = order;
  UpdateGfxCamera();
}

int CameraState::GetOrder() const
{
  return m_order;
}


void CameraState::SetRenderTarget(iRenderTarget2D *renderTarget)
{
  CE_SET(m_renderTarget, renderTarget);
  UpdateGfxCamera();
}

iRenderTarget2D* CameraState::GetRenderTarget()
{
  return m_renderTarget;
}

const iRenderTarget2D* CameraState::GetRenderTarget() const
{
  return m_renderTarget;
}



void CameraState::SetClearMode(eClearMode clearMode)
{
  m_clearMode = clearMode;
  UpdateGfxCamera();
}

eClearMode CameraState::GetClearMode() const
{
  return m_clearMode;
}

void CameraState::SetClearColor(const Color4f &clearColor)
{
  m_clearColor = clearColor;
  UpdateGfxCamera();
}

const Color4f &CameraState::GetClearColor() const
{
  return m_clearColor;
}

void CameraState::SetClearDepth(float clearDepth)
{
  m_clearDepth = clearDepth;
  UpdateGfxCamera();
}

float CameraState::GetClearDepth() const
{
  return m_clearDepth;
}

void CameraState::SetRenderShadows(bool renderShadows)
{
  m_renderShadows = renderShadows;
  UpdateGfxCamera();
}

bool CameraState::IsRenderShadows() const
{
  return m_renderShadows;
}

void CameraState::SetPostProcessing(ce::PostProcessing *postProcessing)
{
  CE_SET(m_postProcessing, postProcessing);
  UpdateGfxCamera();
}

ce::PostProcessing* CameraState::GetPostPRocessing() const
{
  return m_postProcessing;
}

const Camera &CameraState::GetCamera() const
{
  return m_camera;
}

const Projector &CameraState::GetProjector() const
{
  return m_projector;
}


void CameraState::OnAttachedToWorld(World *world)
{
  iGfxScene *scene = world->GetScene();
  scene->Add(m_gfxCamera);
}

void CameraState::OnDetachedFromWorld(World *world)
{
  iGfxScene *scene = world->GetScene();
  scene->Remove(m_gfxCamera);
}

void CameraState::TransformationUpdatedPreChildren()
{
  const Matrix4f &m = GetGlobalMatrix();

  Vector3f eye(m * Vector4f(0.0, 0.0, 0.0, 1.0f));
  Vector3f dir(m * Vector4f(0.0, 0.0, 1.0, 0.0f));
  Vector3f up(m * Vector4f(0.0, 1.0, 0.0, 0.0f));

  m_camera.SetEye(eye);
  m_camera.SetSpot(eye + dir);
  m_camera.SetUp(up);
}

void CameraState::UpdateGfxCamera()
{
  m_gfxCamera->SetOrder(m_order);
  m_gfxCamera->SetRenderTarget(m_renderTarget);
  m_gfxCamera->SetRenderShadows(m_renderShadows);
  m_gfxCamera->SetPostProcessing(m_postProcessing);
  m_gfxCamera->UpdateData(m_near, m_far, m_angle, m_angleWidthHeight);
  m_gfxCamera->UpdateClear(m_clearMode, m_clearColor, m_clearDepth);
}

}