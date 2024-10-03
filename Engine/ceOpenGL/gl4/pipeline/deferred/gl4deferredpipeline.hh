
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardpipeline.hh>
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferreddirectionallightrenderer.hh>
#include <ceOpenGL/gl4/pipeline/deferred/gl4deferredpointlightrenderer.hh>
#include <ceCore/graphics/irenderpipeline.hh>
#include <ceCore/graphics/scene/gfxscenecollector.hh>
#include <ceCore/graphics/skybox/skyboxmesh.hh>

namespace ce
{

class Camera;
class Projector;
class GBuffer;
struct iClipper;
struct iDirectionalLight;
struct iSkyboxRenderer;

namespace opengl
{

class GL4DeferredDirectionalLightRenderer;

CS_CLASS()
class CS_OGL_API GL4DeferredPipeline : public CS_SUPER(iRenderPipeline)
{
CS_CLASS_GEN_OBJECT;
public:
  GL4DeferredPipeline();
  ~GL4DeferredPipeline() noexcept override;


  void Initialize() override;

  void Render(iRenderTarget2D *target, const GfxCamera *camera, iDevice *device, iGfxScene *scene) override;

  void SetRenderMode(int mode)
  { m_renderMode = mode; }
  int GetRenderMode() const
  { return m_renderMode; }

  void IncRenderMode()
  { m_renderMode = m_renderMode == 3 ? 0 : m_renderMode + 1; }
  void DecRenderMode()
  { m_renderMode = m_renderMode == 0 ? 3 : m_renderMode - 1; }

  GL4DeferredPointLightRenderer &GetPointLightRenderer()
  {
    return m_pointLightRenderer;
  }

  GL4DeferredDirectionalLightRenderer &GetDirectionalLightRenderer()
  {
    return m_directionalLightRenderer;
  }

private:

  void RenderGBuffer(uint16_t width, uint16_t height);
  void RenderBackground();
  void PrepareSkybox(iSkyboxRenderer *skyboxRenderer);
  void RenderSkybox(iSkyboxRenderer *skyboxRenderer);
  void RenderBackMask();

  void ScanLightsAndShadows(iClipper *clipper);

  void RenderLighting();
  void RenderLights();
  void RenderLight(GfxLight *light);
  void RenderDirectionalLight( GL4DirectionalLight *directionalLight);
  void RenderPointLight(const GL4PointLight *pointLight);
  void RenderTransparent();
  void RenderForwardMeshShaded(GfxMesh *mesh, std::array<const GfxLight *, MaxLights> &lights, Size offset);
  void RenderForwardMeshUnlit(GfxMesh *mesh);
  void AppendLights(GfxMesh *mesh, const std::vector<GfxLight *> &lights) const;
  size_t AssignLights(const std::vector<GfxMesh::Light> &static_lights,
                      const std::vector<GfxMesh::Light> &dynamic_lights,
                      std::array<const GfxLight *, MaxLights> &lights,
                      size_t offset);

  float CalcMeshLightInfluence(const GfxLight *light, const GfxMesh *mesh) const;
  std::vector<GfxMesh::Light> CalcMeshLightInfluences(const GfxMesh *mesh,
                                                      const std::vector<GfxLight *> &lights,
                                                      bool sorted) const;

  void RenderPostProcessing(iRenderTarget2D *target);
  void Cleanup();


  bool SetupVariables(iRenderTarget2D *target, const GfxCamera *camera, iDevice *device, iGfxScene *scene);
  iRenderTarget2D *UpdateRenderTarget(iDevice *device, iRenderTarget2D *target);
  void UpdateIntermediate();
  void UpdateTransparentTarget();
  void ScanVisibleMeshes(iClipper *clipper);
  void BindCamera();

  GBuffer *m_gBuffer;

  uint64_t        m_frame   = 0;
  iDevice         *m_device;
  const GfxCamera *m_gfxCamera;
  const Camera    *m_camera;
  const Projector *m_projector;
  iGfxScene       *m_scene;
  iRenderTarget2D *m_intermediate;
  iRenderTarget2D *m_target = nullptr;
  iRenderTarget2D *m_transparentTarget = nullptr;


  std::vector<GfxLight *>                 m_globalLights;
  std::vector<GfxLight *>                 m_dynamicLights;
  std::vector<GfxLight *>                 m_staticLights;
  std::vector<GfxLight *>                 m_staticLightsNew;
  std::array<const GfxLight *, MaxLights> m_renderLights = {};
  size_t                                  m_numberOfFixedLights;


  GfxSceneCollector      m_collector;
  std::vector<GfxMesh *> m_shadedMeshes;
  std::vector<GfxMesh *> m_transparentMeshes;
  std::vector<GfxMesh *> m_unshadedMeshes;

  GL4DeferredDirectionalLightRenderer m_directionalLightRenderer;
  GL4DeferredPointLightRenderer       m_pointLightRenderer;

  iShader          *m_backMaskShader;
  iShaderAttribute *m_attrBackMaskDepth;
  int              m_renderMode;
  SkyboxMesh       m_skyboxMesh;
};


}

}