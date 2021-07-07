
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcOpenGL/gl4/pipeline/forward/gl4forwardpointlightrenderer.hh>
#include <spcOpenGL/gl4/pipeline/forward/gl4forwarddirectionallightrenderer.hh>
#include <spcCore/graphics/camera.hh>
#include <spcCore/graphics/irenderpipeline.hh>
#include <spcCore/graphics/projector.hh>
#include <spcCore/graphics/scene/gfxmesh.hh>
#include <spcCore/types.hh>
#include <map>
#include <vector>

namespace spc
{

class GfxLight;
class GfxMesh;

struct iSampler;

namespace opengl
{

class GL4DirectionalLight;
class GL4PointLight;
class GL4RenderTargetCube;

SPC_CLASS()
class SPC_OGL_API GL4ForwardPipeline : public SPC_SUPER(iRenderPipeline)
{
  SPC_CLASS_GEN_OBJECT;
public:
  GL4ForwardPipeline();
  ~GL4ForwardPipeline() noexcept override;

  void Initialize() override;

  void Render(iRenderTarget2D * taget, const Camera & camera, const Projector & projector, iDevice * device, GfxScene * scene) override;

private:

  void LightScanned(GfxLight * light);
  void RenderMesh(GfxMesh * mesh, const GfxLight * *lights, Size offset);

  void CollectShadowLights(GfxLight * light);
  void RenderShadowMaps();


  Size AssignLights(const std::vector<GfxMesh::Light> &staticLights,
    const std::vector<GfxMesh::Light> &dynamicLights,
    const GfxLight * *lights,
    Size offset);


  float CalcMeshLightInfluence(const GfxLight * light, const GfxMesh * mesh) const;
  std::vector<GfxMesh::Light> CalcMeshLightInfluences(const GfxMesh * mesh, const std::vector<GfxLight*> &lights, bool sorted) const;
  void AppendLights(GfxMesh * mesh, const std::vector<GfxLight*> &lights) const;



  UInt64 m_frame;
  iDevice* m_device;
  Camera m_camera;
  Projector m_projector;
  GfxScene* m_scene;
  iRenderTarget2D* m_target;

  
  std::vector<GfxLight*> m_dynamicLights;
  std::vector<GfxLight*> m_staticLights;
  std::vector<GfxLight*> m_staticLightsNew;

  GL4ForwardPointLightRenderer m_pointLightRenderer;
  GL4ForwardDirectionalLightRenderer m_directionalLightRenderer;


};

}

}
