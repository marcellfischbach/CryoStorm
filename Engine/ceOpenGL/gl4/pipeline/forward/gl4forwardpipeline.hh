
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/openglconstants.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardpointlightrenderer.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwarddirectionallightrenderer.hh>
#include <ceCore/graphics/csCamera.hh>
#include <ceCore/graphics/iRenderPipeline.hh>
#include <ceCore/graphics/csProjector.hh>
#include <ceCore/graphics/skybox/csSkyboxMesh.hh>
#include <ceCore/graphics/scene/csGfxMesh.hh>
#include <ceCore/graphics/scene/csGfxSceneCollector.hh>
#include <ceCore/types.hh>
#include <map>
#include <vector>

namespace cryo
{

class csGfxLight;
class csGfxMesh;

struct iClipper;
struct iSampler;
struct iSkyboxRenderer;

namespace opengl
{

class GL4DirectionalLight;
class GL4PointLight;
class GL4RenderTargetCube;

CS_CLASS()
class CS_OGL_API GL4ForwardPipeline : public CS_SUPER(iRenderPipeline)
{
  CS_CLASS_GEN_OBJECT;
public:
  GL4ForwardPipeline();
  ~GL4ForwardPipeline() noexcept override;

  void Initialize() override;

  void Render(iRenderTarget2D * taget, const csGfxCamera * camera, iDevice * device, iGfxScene * scene) override;

protected:
  void PrepareSkybox(iSkyboxRenderer *skyboxRenderer);
  void RenderSkybox(iSkyboxRenderer *skyboxRenderer);
  void RenderPostProcessing(iRenderTarget2D *target);


private:
  bool SetupVariables(iRenderTarget2D *target, const csGfxCamera *camera, iDevice *device, iGfxScene *scene);
  void CollectLightsAndShadows (iClipper *clipper);
  void ScanVisibleMeshes(iClipper* clipper);
  void BindCamera();
  void RenderDepthToTarget ();
  void RenderBackground();
  void RenderOpaqueMeshes();
  void RenderTransparentMeshes ();
  void RenderDebugToTarget ();
  void ApplyDepthBufferToLightRenderers();
  void Cleanup ();


  iRenderTarget2D *UpdateRenderTarget(cryo::iDevice *device, cryo::iRenderTarget2D *target);

  void LightScanned(csGfxLight * light);
  void RenderUnlitDepthMesh(csGfxMesh * mesh);
  void RenderUnlitForwardMesh(csGfxMesh * mesh);
  void RenderMesh(csGfxMesh * mesh, std::array<const csGfxLight*, MaxLights> &lights, Size offset);

  void CollectShadowLights(csGfxLight * light);
  void RenderShadowMaps();


  Size AssignLights(const std::vector<csGfxMesh::Light> &staticLights,
    const std::vector<csGfxMesh::Light> &dynamicLights,
    std::array<const csGfxLight *, MaxLights> &lights,
    Size offset);


  float CalcMeshLightInfluence(const csGfxLight * light, const csGfxMesh * mesh) const;
  std::vector<csGfxMesh::Light> CalcMeshLightInfluences(const csGfxMesh * mesh, const std::vector<csGfxLight*> &lights, bool sorted) const;
  void AppendLights(csGfxMesh * mesh, const std::vector<csGfxLight*> &lights) const;


protected:
  uint64_t m_frame;
  iDevice* m_device;
  const csGfxCamera *m_gfxCamera;
  const csCamera    *m_camera;
  const csProjector *m_projector;
  iGfxScene  * m_scene;
  iRenderTarget2D * m_target;
private:
  std::array<const csGfxLight *, MaxLights> m_renderLights = {};
  size_t                                    m_numberOfFixedLights;



  std::vector<csGfxLight*> m_dynamicLights;
  std::vector<csGfxLight*> m_staticLights;
  std::vector<csGfxLight*> m_staticLightsNew;

  csGfxSceneCollector     m_collector;
  std::vector<csGfxMesh*> m_shadedMeshes;
  std::vector<csGfxMesh*> m_transparentMeshes;
  std::vector<csGfxMesh*> m_unshadedMeshes;


  GL4ForwardPointLightRenderer m_pointLightRenderer;
  GL4ForwardDirectionalLightRenderer m_directionalLightRenderer;
  csSkyboxMesh                       m_skyboxMesh;


};

}

}
