
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/entity/csSpatialState.hh>

#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/eClearMode.hh>
#include <csCore/math/csColor4f.hh>

namespace cs
{

class csGfxCamera;
struct iRenderTarget2D;
struct iSkyboxRenderer;
class csPostProcessing;

CS_CLASS(jclass="org.cryo.core.entity.CsCameraState")
class CS_CORE_API csCameraState : public CS_SUPER(csSpatialState)
{
CS_CLASS_GEN_OBJECT;
public:
  csCameraState();
  ~csCameraState() override;


  CS_FUNCTION()
  void SetNear(float near);
  CS_FUNCTION()
  float GetNear() const;

  CS_FUNCTION()
  void SetFar(float near);
  CS_FUNCTION()
  float GetFar() const;

  CS_FUNCTION()
  void SetAngle(float angle);
  CS_FUNCTION()
  float GetAngle() const;

  CS_FUNCTION()
  void SetAngleWidthHeight(float angleWidtHeight);
  CS_FUNCTION()
  float GetAngleWidthHeight() const;


  CS_FUNCTION()
  void SetOrder(int order);
  CS_FUNCTION()
  int GetOrder() const;

  CS_FUNCTION()
  void SetRenderTarget(cs::iRenderTarget2D *renderTarget);
  CS_FUNCTION()
  cs::iRenderTarget2D *GetRenderTarget();
  const iRenderTarget2D *GetRenderTarget() const;

  CS_FUNCTION(jenum)
  void SetClearMode(cs::eClearMode clearMode);
  CS_FUNCTION(jenum)
  cs::eClearMode GetClearMode() const;

  CS_FUNCTION(jenum)
  void SetClearColorMode(cs::eClearColorMode clearMode);
  CS_FUNCTION(jenum)
  cs::eClearColorMode GetClearColorMode() const;

  CS_FUNCTION()
  void SetClearColor(const cs::csColor4f &clearColor);
  CS_FUNCTION()
  const cs::csColor4f &GetClearColor() const;

  CS_FUNCTION()
  void SetClearDepth(float clearDepth);
  CS_FUNCTION()
  float GetClearDepth() const;

  CS_FUNCTION()
  void SetRenderShadows(bool renderShadows);
  CS_FUNCTION()
  bool IsRenderShadows() const;

  void SetPostProcessing(csPostProcessing *postProcessing);
  csPostProcessing *GetPostPRocessing() const;

  void SetSkyboxRenderer(iSkyboxRenderer *skyboxRenderer);
  iSkyboxRenderer *GetSkyboxRenderer() const;

  const csCamera &GetCamera() const;
  const csProjector &GetProjector() const;

  CS_FUNCTION()
  void Update(uint32_t width, uint32_t height);

  void OnAttachedToWorld(csWorld *world) override;
  void OnDetachedFromWorld(csWorld *world) override;

protected:
  void TransformationUpdatedPreChildren() override;

private:
  float m_near;
  float m_far;

  float m_angle;

  float m_angleWidthHeight;

  int m_order;

  csRef<iRenderTarget2D> m_renderTarget;

  eClearMode m_clearMode = eClearMode::DepthColor;

  eClearColorMode m_clearColorMode = eClearColorMode::PlainColor;

  csColor4f m_clearColor = csColor4f();

  float m_clearDepth = 1.0f;

  bool m_renderShadows = true;


  csCamera         m_camera;
  csProjector      m_projector;
  csRef<csPostProcessing> m_postProcessing;
  csRef<iSkyboxRenderer>  m_skyboxRenderer;

  void UpdateGfxCamera();
  csRef<csGfxCamera> m_gfxCamera;


};


}