
#pragma once

#include <ceOpenGL/openglexport.hh>

#include <ceCore/graphics/irenderpipeline.hh>
#include <ceCore/graphics/scene/gfxscenecollector.hh>

namespace ce
{

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

  void Render(iRenderTarget2D * target, const GfxCamera * camera, iDevice * device, iGfxScene * scene) override;


private:

  void SetupVariables(iRenderTarget2D* target, const GfxCamera* camera, iDevice* device, iGfxScene* scene);
  void ScanVisibleMeshes(iClipper* clipper);

  GBuffer *m_gBuffer;

  iGfxScene *m_scene;

  GfxSceneCollector m_collector;
  std::vector<GfxMesh*> m_shadedMeshes;
  std::vector<GfxMesh*> m_transparentMeshes;
  std::vector<GfxMesh*> m_unshadedMeshes;
};




}

}