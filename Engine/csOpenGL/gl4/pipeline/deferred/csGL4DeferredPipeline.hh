
#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csOpenGL/gl4/pipeline/forward/csGL4ForwardPipeline.hh>
#include <csOpenGL/gl4/pipeline/deferred/csGL4DeferredDirectionalLightRenderer.hh>
#include <csOpenGL/gl4/pipeline/deferred/csGL4DeferredPointLightRenderer.hh>
#include <csCore/graphics/iRenderPipeline.hh>
#include <csCore/graphics/scene/csGfxSceneCollector.hh>
#include <csCore/graphics/skybox/csSkyboxMesh.hh>

namespace cryo
{

class csCamera;
class csProjector;
class csGBuffer;
struct iClipper;
struct iDirectionalLight;
struct iSkyboxRenderer;

namespace opengl
{

class csGL4DeferredDirectionalLightRenderer;

CS_CLASS()
class CS_OGL_API csGL4DeferredPipeline : public CS_SUPER(iRenderPipeline)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4DeferredPipeline();
  ~csGL4DeferredPipeline() noexcept override;


  void Initialize() override;

  void Render(iRenderTarget2D *target, const csGfxCamera *camera, iDevice *device, iGfxScene *scene) override;

  void SetRenderMode(int mode)
  { m_renderMode = mode; }
  int GetRenderMode() const
  { return m_renderMode; }

  void IncRenderMode()
  { m_renderMode = m_renderMode == 3 ? 0 : m_renderMode + 1; }
  void DecRenderMode()
  { m_renderMode = m_renderMode == 0 ? 3 : m_renderMode - 1; }

  csGL4DeferredPointLightRenderer &GetPointLightRenderer()
  {
    return m_pointLightRenderer;
  }

  csGL4DeferredDirectionalLightRenderer &GetDirectionalLightRenderer()
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
  void RenderLight(csGfxLight *light);
  void RenderDirectionalLight(csGL4DirectionalLight *directionalLight);
  void RenderPointLight(const csGL4PointLight *pointLight);
  void RenderTransparent();
  void RenderForwardMeshShaded(csGfxMesh *mesh, std::array<const csGfxLight *, MaxLights> &lights, Size offset);
  void RenderForwardMeshUnlit(csGfxMesh *mesh);
  void AppendLights(csGfxMesh *mesh, const std::vector<csGfxLight *> &lights) const;
  size_t AssignLights(const std::vector<csGfxMesh::Light> &static_lights,
                      const std::vector<csGfxMesh::Light> &dynamic_lights,
                      std::array<const csGfxLight *, MaxLights> &lights,
                      size_t offset);

  float CalcMeshLightInfluence(const csGfxLight *light, const csGfxMesh *mesh) const;
  std::vector<csGfxMesh::Light> CalcMeshLightInfluences(const csGfxMesh *mesh,
                                                        const std::vector<csGfxLight *> &lights,
                                                        bool sorted) const;

  void RenderPostProcessing(iRenderTarget2D *target);
  void Cleanup();


  bool SetupVariables(iRenderTarget2D *target, const csGfxCamera *camera, iDevice *device, iGfxScene *scene);
  iRenderTarget2D *UpdateRenderTarget(iDevice *device, iRenderTarget2D *target);
  void UpdateIntermediate();
  void UpdateTransparentTarget();
  void ScanVisibleMeshes(iClipper *clipper);
  void BindCamera();

  csGBuffer *m_gBuffer;

  uint64_t        m_frame   = 0;
  iDevice           *m_device;
  const csGfxCamera *m_gfxCamera;
  const csCamera    *m_camera;
  const csProjector *m_projector;
  iGfxScene         *m_scene;
  iRenderTarget2D *m_intermediate;
  iRenderTarget2D *m_target = nullptr;
  iRenderTarget2D *m_transparentTarget = nullptr;


  std::vector<csGfxLight *>                 m_globalLights;
  std::vector<csGfxLight *>                 m_dynamicLights;
  std::vector<csGfxLight *>                 m_staticLights;
  std::vector<csGfxLight *>                 m_staticLightsNew;
  std::array<const csGfxLight *, MaxLights> m_renderLights = {};
  size_t                                    m_numberOfFixedLights;


  csGfxSceneCollector      m_collector;
  std::vector<csGfxMesh *> m_shadedMeshes;
  std::vector<csGfxMesh *> m_transparentMeshes;
  std::vector<csGfxMesh *> m_unshadedMeshes;

  csGL4DeferredDirectionalLightRenderer m_directionalLightRenderer;
  csGL4DeferredPointLightRenderer       m_pointLightRenderer;

  iShader          *m_backMaskShader;
  iShaderAttribute *m_attrBackMaskDepth;
  int          m_renderMode;
  csSkyboxMesh m_skyboxMesh;
};


}

}