
#pragma once

#include <ceOpenGL/openglexport.hh>

#include <ceCore/graphics/irenderpipeline.hh>
#include <ceCore/graphics/scene/gfxscenecollector.hh>

namespace ce
{

class Camera;
class Projector;
class GBuffer;
struct iClipper;

namespace opengl
{


CE_CLASS()
class CE_OGL_API GL4DeferredPipeline : public CE_SUPER(iRenderPipeline)
{
CE_CLASS_GEN_OBJECT;
public:
  GL4DeferredPipeline();
  ~GL4DeferredPipeline() noexcept override;


  void Initialize() override;

  void Render(iRenderTarget2D *target, const GfxCamera *camera, iDevice *device, iGfxScene *scene) override;


private:

  void RenderGBuffer(iDevice *device, const GfxCamera *camera, iGfxScene *scene, uint16_t width, uint16_t height);


  void SetupVariables(iRenderTarget2D *target, const GfxCamera *camera, iDevice *device, iGfxScene *scene);
  void ScanVisibleMeshes(iClipper *clipper);
  void BindCamera();

  GBuffer *m_gBuffer;

  iDevice         *m_device;
  const GfxCamera *m_gfxCamera;
  const Camera    *m_camera;
  const Projector *m_projector;
  iGfxScene       *m_scene;
  iRenderTarget2D *m_target;

  GfxSceneCollector      m_collector;
  std::vector<GfxMesh *> m_shadedMeshes;
  std::vector<GfxMesh *> m_transparentMeshes;
  std::vector<GfxMesh *> m_unshadedMeshes;
};


}

}