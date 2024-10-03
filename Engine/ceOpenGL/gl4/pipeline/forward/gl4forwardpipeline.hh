
#pragma once

#include <ceOpenGL/openglexport.hh>
#include <ceOpenGL/openglconstants.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwardpointlightrenderer.hh>
#include <ceOpenGL/gl4/pipeline/forward/gl4forwarddirectionallightrenderer.hh>
#include <ceCore/graphics/camera.hh>
#include <ceCore/graphics/irenderpipeline.hh>
#include <ceCore/graphics/projector.hh>
#include <ceCore/graphics/skybox/skyboxmesh.hh>
#include <ceCore/graphics/scene/gfxmesh.hh>
#include <ceCore/graphics/scene/gfxscenecollector.hh>
#include <ceCore/types.hh>
#include <map>
#include <vector>

namespace ce
{

class GfxLight;
class GfxMesh;

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

  void Render(iRenderTarget2D * taget, const GfxCamera * camera, iDevice * device, iGfxScene * scene) override;

protected:
  void PrepareSkybox(iSkyboxRenderer *skyboxRenderer);
  void RenderSkybox(iSkyboxRenderer *skyboxRenderer);
  void RenderPostProcessing(iRenderTarget2D *target);


private:
  bool SetupVariables(iRenderTarget2D *target,  const GfxCamera *camera,iDevice *device,iGfxScene *scene);
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


  iRenderTarget2D *UpdateRenderTarget(ce::iDevice *device, ce::iRenderTarget2D *target);

  void LightScanned(GfxLight * light);
  void RenderUnlitDepthMesh(GfxMesh * mesh);
  void RenderUnlitForwardMesh(GfxMesh * mesh);
  void RenderMesh(GfxMesh * mesh, std::array<const GfxLight*, MaxLights> &lights, Size offset);

  void CollectShadowLights(GfxLight * light);
  void RenderShadowMaps();


  Size AssignLights(const std::vector<GfxMesh::Light> &staticLights,
    const std::vector<GfxMesh::Light> &dynamicLights,
    std::array<const GfxLight *, MaxLights> &lights,
    Size offset);


  float CalcMeshLightInfluence(const GfxLight * light, const GfxMesh * mesh) const;
  std::vector<GfxMesh::Light> CalcMeshLightInfluences(const GfxMesh * mesh, const std::vector<GfxLight*> &lights, bool sorted) const;
  void AppendLights(GfxMesh * mesh, const std::vector<GfxLight*> &lights) const;


protected:
  uint64_t m_frame;
  iDevice* m_device;
  const GfxCamera *m_gfxCamera;
  const Camera *m_camera;
  const Projector *m_projector;
  iGfxScene  * m_scene;
  iRenderTarget2D * m_target;
private:
  std::array<const GfxLight *, MaxLights> m_renderLights = {};
  size_t m_numberOfFixedLights;



  std::vector<GfxLight*> m_dynamicLights;
  std::vector<GfxLight*> m_staticLights;
  std::vector<GfxLight*> m_staticLightsNew;

  GfxSceneCollector m_collector;
  std::vector<GfxMesh*> m_shadedMeshes;
  std::vector<GfxMesh*> m_transparentMeshes;
  std::vector<GfxMesh*> m_unshadedMeshes;


  GL4ForwardPointLightRenderer m_pointLightRenderer;
  GL4ForwardDirectionalLightRenderer m_directionalLightRenderer;
  SkyboxMesh m_skyboxMesh;


};

}

}
