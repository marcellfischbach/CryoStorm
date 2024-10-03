#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/eclearmode.hh>
#include <ceCore/math/color4f.hh>

namespace cryo
{

class Camera;
class Projector;
struct iRenderTarget2D;
struct iTextureCube;
struct iSkyboxRenderer;
class PostProcessing;

CS_CLASS()
class CS_CORE_API GfxCamera : public CS_SUPER(iObject)
{
CS_CLASS_GEN_OBJECT;
public:
  GfxCamera();
  ~GfxCamera() override;

  void SetCamera(Camera *camera);
  const Camera *GetCamera() const;
  Camera *GetCamera();

  void SetProjector(Projector *projector);
  const Projector *GetProjector() const;
  Projector *GetProjector();

  eClearMode GetClearMode() const;
  eClearColorMode GetClearColorMode() const;
  const Color4f &GetClearColor() const;
  float GetClearDepth() const;

  void SetRenderTarget(iRenderTarget2D *renderTarget);
  iRenderTarget2D *GetRenderTarget();
  const iRenderTarget2D *GetRenderTarget() const;

  void SetOrder(int order);
  int GetOrder() const;

  void SetRenderShadows(bool renderShadows);
  bool IsRenderShadows() const;

  void SetPostProcessing(PostProcessing *postProcessing);
  PostProcessing *GetPostProcessing() const;

  void SetSkyboxRenderer(iSkyboxRenderer *skyboxRenderer);
  iSkyboxRenderer* GetSkyboxRenderer() const;

  void UpdateData(float near, float far, float angle, float angleWidthHeight);

  void UpdateClear(eClearMode mode, eClearColorMode clearColorMode, const Color4f &color, iSkyboxRenderer *skybox, float depth);

  void UpdateProjector(uint32_t width, uint32_t height);

private:

  float m_near;
  float m_far;
  float m_angle;
  float m_angleWidthHeight;


  eClearMode      m_clearMode;
  eClearColorMode m_clearColorMode;

  Color4f      m_clearColor;
  float        m_clearDepth;

  Camera    *m_camera;
  Projector *m_projector;

  iSkyboxRenderer *m_skyboxRenderer;

  iRenderTarget2D *m_renderTarget;
  int             m_order;
  bool            m_renderShadows;

  PostProcessing *m_postProcessing;
};


}

