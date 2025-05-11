
#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csOpenGL/csOpenglConstants.hh>
#include <csOpenGL/gl4/pipeline/forward/csGL4ForwardPointLightRenderer.hh>
#include <csOpenGL/gl4/pipeline/forward/csGL4ForwardDirectionalLightRenderer.hh>
#include <csCore/graphics/csCamera.hh>
#include <csCore/graphics/iRenderPipeline.hh>
#include <csCore/graphics/csProjector.hh>
#include <csCore/graphics/skybox/csSkyboxMesh.hh>
#include <csCore/graphics/scene/csGfxMesh.hh>
#include <csCore/graphics/scene/csGfxSceneCollector.hh>
#include <csCore/csTypes.hh>
#include <map>
#include <vector>

namespace cs
{

class csGfxLight;
class csGfxMesh;

struct iClipper;
struct iSampler;
struct iSkyboxRenderer;

namespace opengl
{

class csGL4DirectionalLight;
class csGL4PointLight;
class csGL4RenderTargetCube;

CS_CLASS()
class CS_OGL_API csGL4ForwardPipeline : public CS_SUPER(iForwardRenderPipeline)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4ForwardPipeline();
  ~csGL4ForwardPipeline() noexcept override;

  void Initialize() override;
  void Shutdown () override;

  void Render(iRenderTarget2D *taget, const csGfxCamera *camera, iDevice *device, iGfxScene *scene) override;

protected:
  void PrepareSkybox(iSkyboxRenderer *skyboxRenderer);
  void RenderSkybox(iSkyboxRenderer *skyboxRenderer);
  void RenderPostProcessing(iRenderTarget2D *target);


private:
  bool SetupVariables(iRenderTarget2D *target, const csGfxCamera *camera, iDevice *device, iGfxScene *scene);
  void CollectLightsAndShadows(iClipper *clipper);
  void ScanVisibleMeshes(iClipper *clipper);
  void BindCamera();
  void RenderDepthToTarget();
  void RenderBackground();
  void RenderOpaqueMeshes();
  void RenderTransparentMeshes();
  void RenderDebugToTarget();
  void ApplyDepthBufferToLightRenderers();
  void Cleanup();


  iRenderTarget2D *UpdateRenderTarget(cs::iDevice *device, cs::iRenderTarget2D *target);

  void LightScanned(csGfxLight *light);
  void RenderUnlitDepthMesh(csGfxMesh *mesh);
  void RenderUnlitForwardMesh(csGfxMesh *mesh);
  void RenderMesh(csGfxMesh *mesh, std::array<const csGfxLight *, MaxLights> &lights, Size offset);

  void CollectShadowLights(csGfxLight *light);
  void RenderShadowMaps();


  Size AssignLights(const std::vector<csGfxMesh::Light> &staticLights,
                    const std::vector<csGfxMesh::Light> &dynamicLights,
                    std::array<const csGfxLight *, MaxLights> &lights,
                    Size offset);


  float CalcMeshLightInfluence(const csGfxLight *light, const csGfxMesh *mesh) const;
  std::vector<csGfxMesh::Light>
  CalcMeshLightInfluences(const csGfxMesh *mesh, const std::vector<csGfxLight *> &lights, bool sorted) const;
  void AppendLights(csGfxMesh *mesh, const std::vector<csGfxLight *> &lights) const;


protected:
  uint64_t m_frame;
  csRef<iDevice> m_device;
  csRef<const csGfxCamera> m_gfxCamera;
  csRef<const csCamera> m_camera;
  csRef<const csProjector> m_projector;
  csRef<iGfxScene> m_scene;
  csRef<iRenderTarget2D> m_target;
private:
  std::array<const csGfxLight *, MaxLights> m_renderLights = {};
  size_t m_numberOfFixedLights;


  std::vector<csGfxLight *> m_dynamicLights;
  std::vector<csGfxLight *> m_staticLights;
  std::vector<csGfxLight *> m_staticLightsNew;

  csGfxSceneCollector m_collector;

  csGL4ForwardPointLightRenderer m_pointLightRenderer;
  csGL4ForwardDirectionalLightRenderer m_directionalLightRenderer;
  csSkyboxMesh m_skyboxMesh;


};

}

}
