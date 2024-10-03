

#include <ceCore/entity/csCameraState.hh>
#include <ceCore/math/matrix4f.hh>
#include <ceCore/math/vector3f.hh>
#include <ceCore/math/vector4f.hh>
#include <math.h>
#include <ceCore/graphics/iRenderTarget2D.hh>
#include <ceCore/graphics/iSkyboxRenderer.hh>
#include <ceCore/graphics/csPostProcessing.hh>
#include <ceCore/graphics/scene/csGfxCamera.hh>
#include <ceCore/graphics/scene/iGfxScene.hh>
#include <ceCore/entity/csWorld.hh>

namespace cryo
{


csCameraState::csCameraState()
    : csSpatialState(),
      m_near(1.0f),
      m_far(1024.0f),
      m_angle((float) M_PI / 4.0f),
      m_angleWidthHeight(0.0f),
      m_gfxCamera(new csGfxCamera()),
      m_order(0),
      m_renderTarget(nullptr),
      m_skyboxRenderer(nullptr),
      m_postProcessing(nullptr)
{
  CS_CLASS_GEN_CONSTR;
  m_gfxCamera->SetCamera(&m_camera);
  m_gfxCamera->SetProjector(&m_projector);
  UpdateGfxCamera();
}

csCameraState::~csCameraState()
{
  m_gfxCamera->Release();
}

void csCameraState::SetNear(float near)
{
  m_near = near;
  UpdateGfxCamera();
}

float csCameraState::GetNear() const
{
  return m_near;
}

void csCameraState::SetFar(float far)
{
  m_far = far;
  UpdateGfxCamera();
}

float csCameraState::GetFar() const
{
  return m_far;
}

void csCameraState::SetAngle(float angle)
{
  m_angle = angle;
  UpdateGfxCamera();
}

float csCameraState::GetAngle() const
{
  return m_angle;
}

void csCameraState::SetAngleWidthHeight(float angleWidthHeight)
{
  m_angleWidthHeight = angleWidthHeight;
  UpdateGfxCamera();
}

float csCameraState::GetAngleWidthHeight() const
{
  return m_angleWidthHeight;
}

void csCameraState::SetOrder(int order)
{
  m_order = order;
  UpdateGfxCamera();
}

int csCameraState::GetOrder() const
{
  return m_order;
}


void csCameraState::SetRenderTarget(iRenderTarget2D *renderTarget)
{
  CS_SET(m_renderTarget, renderTarget);
  UpdateGfxCamera();
}

iRenderTarget2D* csCameraState::GetRenderTarget()
{
  return m_renderTarget;
}

void csCameraState::SetSkyboxRenderer(cryo::iSkyboxRenderer *skyboxRenderer)
{
  CS_SET(m_skyboxRenderer, skyboxRenderer);
}

iSkyboxRenderer* csCameraState::GetSkyboxRenderer() const
{
  return m_skyboxRenderer;
}

const iRenderTarget2D* csCameraState::GetRenderTarget() const
{
  return m_renderTarget;
}



void csCameraState::SetClearMode(eClearMode clearMode)
{
  m_clearMode = clearMode;
  UpdateGfxCamera();
}

eClearMode csCameraState::GetClearMode() const
{
  return m_clearMode;
}




void csCameraState::SetClearColorMode(eClearColorMode clearColorMode)
{
  m_clearColorMode = clearColorMode;
  UpdateGfxCamera();
}

eClearColorMode csCameraState::GetClearColorMode() const
{
  return m_clearColorMode;
}


void csCameraState::SetClearColor(const Color4f &clearColor)
{
  m_clearColor = clearColor;
  UpdateGfxCamera();
}

const Color4f &csCameraState::GetClearColor() const
{
  return m_clearColor;
}

void csCameraState::SetClearDepth(float clearDepth)
{
  m_clearDepth = clearDepth;
  UpdateGfxCamera();
}

float csCameraState::GetClearDepth() const
{
  return m_clearDepth;
}

void csCameraState::SetRenderShadows(bool renderShadows)
{
  m_renderShadows = renderShadows;
  UpdateGfxCamera();
}

bool csCameraState::IsRenderShadows() const
{
  return m_renderShadows;
}

void csCameraState::SetPostProcessing(cryo::csPostProcessing *postProcessing)
{
  CS_SET(m_postProcessing, postProcessing);
  UpdateGfxCamera();
}

cryo::csPostProcessing* csCameraState::GetPostPRocessing() const
{
  return m_postProcessing;
}

const csCamera &csCameraState::GetCamera() const
{
  return m_camera;
}

const csProjector &csCameraState::GetProjector() const
{
  return m_projector;
}


void csCameraState::OnAttachedToWorld(csWorld *world)
{
  iGfxScene *scene = world->GetScene();
  scene->Add(m_gfxCamera);
}

void csCameraState::OnDetachedFromWorld(csWorld *world)
{
  iGfxScene *scene = world->GetScene();
  scene->Remove(m_gfxCamera);
}

void csCameraState::TransformationUpdatedPreChildren()
{
  const Matrix4f &m = GetGlobalMatrix();

  Vector3f eye(m * Vector4f(0.0, 0.0, 0.0, 1.0f));
  Vector3f dir(m * Vector4f(0.0, 0.0, 1.0, 0.0f));
  Vector3f up(m * Vector4f(0.0, 1.0, 0.0, 0.0f));

  m_camera.SetEye(eye);
  m_camera.SetSpot(eye + dir);
  m_camera.SetUp(up);
}

void csCameraState::UpdateGfxCamera()
{
  m_gfxCamera->SetOrder(m_order);
  m_gfxCamera->SetRenderTarget(m_renderTarget);
  m_gfxCamera->SetRenderShadows(m_renderShadows);
  m_gfxCamera->SetPostProcessing(m_postProcessing);
  m_gfxCamera->UpdateData(m_near, m_far, m_angle, m_angleWidthHeight);
  m_gfxCamera->UpdateClear(m_clearMode, m_clearColorMode, m_clearColor, m_skyboxRenderer, m_clearDepth);
}

}