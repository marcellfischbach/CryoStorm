
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csSpatialState.hh>

#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/eClearMode.hh>
#include <csCore/math/csColor4f.hh>

namespace cryo
{

class csGfxCamera;
struct iRenderTarget2D;
struct iSkyboxRenderer;
class csPostProcessing;

CS_CLASS()
class CS_CORE_API csCameraState : public CS_SUPER(csSpatialState)
{
CS_CLASS_GEN_OBJECT;
public:
  csCameraState();
  ~csCameraState() override;


  void SetNear(float near);
  float GetNear() const;

  void SetFar(float near);
  float GetFar() const;

  void SetAngle(float angle);
  float GetAngle() const;

  void SetAngleWidthHeight(float angleWidtHeight);
  float GetAngleWidthHeight() const;

  void SetOrder(int order);
  int GetOrder() const;

  void SetRenderTarget(iRenderTarget2D *renderTarget);
  iRenderTarget2D *GetRenderTarget();
  const iRenderTarget2D *GetRenderTarget() const;

  void SetClearMode(eClearMode clearMode);
  eClearMode GetClearMode() const;

  void SetClearColorMode(eClearColorMode clearMode);
  eClearColorMode GetClearColorMode() const;

  void SetClearColor(const csColor4f &clearColor);
  const csColor4f &GetClearColor() const;

  void SetClearDepth(float clearDepth);
  float GetClearDepth() const;

  void SetRenderShadows(bool renderShadows);
  bool IsRenderShadows() const;

  void SetPostProcessing(csPostProcessing *postProcessing);
  csPostProcessing *GetPostPRocessing() const;

  void SetSkyboxRenderer(iSkyboxRenderer *skyboxRenderer);
  iSkyboxRenderer *GetSkyboxRenderer() const;

  const csCamera &GetCamera() const;
  const csProjector &GetProjector() const;

  void Update(u32 width, u32 height);

  void OnAttachedToWorld(csWorld *world) override;
  void OnDetachedFromWorld(csWorld *world) override;

protected:
  void TransformationUpdatedPreChildren() override;

private:
  CS_PROPERTY()
  float m_near;
  CS_PROPERTY()
  float m_far;

  CS_PROPERTY()
  float m_angle;

  CS_PROPERTY()
  float m_angleWidthHeight;

  CS_PROPERTY()
  int m_order;

  CS_PROPERTY()
  iRenderTarget2D *m_renderTarget;

  CS_PROPERTY()
  eClearMode m_clearMode = eClearMode::DepthColor;

  CS_PROPERTY()
  eClearColorMode m_clearColorMode = eClearColorMode::PlainColor;

  CS_PROPERTY()
  csColor4f m_clearColor = csColor4f();

  CS_PROPERTY()
  float m_clearDepth = 1.0f;

  CS_PROPERTY()
  bool m_renderShadows = true;


  csCamera         m_camera;
  csProjector      m_projector;
  csPostProcessing *m_postProcessing;
  iSkyboxRenderer  *m_skyboxRenderer;

  void UpdateGfxCamera();
  csGfxCamera *m_gfxCamera;


};


}