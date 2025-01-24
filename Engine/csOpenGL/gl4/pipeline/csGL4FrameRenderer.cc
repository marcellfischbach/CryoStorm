
#include <csOpenGL/gl4/pipeline/csGL4FrameRenderer.hh>
#include <csOpenGL/gl4/pipeline/forward/csGL4ForwardPipeline.hh>
#include <csOpenGL/gl4/pipeline/deferred/csGL4DeferredPipeline.hh>

#include <csCore/graphics/iRenderPipeline.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/scene/csGfxCamera.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/eClearMode.hh>
#include <algorithm>
#include <ranges>

namespace cs::opengl
{


csGL4FrameRenderer::csGL4FrameRenderer()
{

}

csGL4FrameRenderer::~csGL4FrameRenderer()
{
  if (m_renderPipeline)
  {
    m_renderPipeline->Shutdown();
    m_renderPipeline = nullptr;
  }
}

bool gfx_camera_sorter(csGfxCamera *cam0, csGfxCamera *cam1)
{
  iRenderTarget *target0 = cam0->GetRenderTarget();
  iRenderTarget *target1 = cam1->GetRenderTarget();
  if (target0 && !target1)
  {
    return false;
  }
  else if (!target0 && target1)
  {
    return true;
  }
  return cam0->GetOrder() < cam1->GetOrder();
}


void csGL4FrameRenderer::Render(iRenderTarget2D *target, iDevice *device, iGfxScene *scene)
{
  auto                       cameras = std::vector<csRef<csGfxCamera>>(scene->GetCameras());
  std::vector<csGfxCamera *> plainCameras;
  plainCameras.reserve(cameras.size());

  for (const auto &camera: cameras)
  {
    if (camera->GetRenderTarget())
    {
      Render(camera->GetRenderTarget(), camera, device, scene);
    }
    else
    {
      plainCameras.emplace_back(camera);
    }
  }

  std::sort(plainCameras.begin(), plainCameras.end(), gfx_camera_sorter);
  for (auto camera: plainCameras)
  {
    Render(target, camera, device, scene);
  }
}

void csGL4FrameRenderer::Render(iRenderTarget2D *target, csGfxCamera *camera, iDevice *device, iGfxScene *scene)
{
  camera->UpdateProjector(target->GetWidth(), target->GetHeight());
  m_renderPipeline->Render(target, camera, device, scene);
}

void csGL4FrameRenderer::SetRenderPipeline(iRenderPipeline *renderPipeline)
{
  m_renderPipeline = renderPipeline;
}


csOwned<iFrameRenderer> csGL4FrameRendererFactory::Create(cs::eRenderPipeline pipeline) const
{
  iRenderPipeline *renderPipeline = nullptr;
  switch (pipeline)
  {
    case eRenderPipeline::eRP_Forward:
      renderPipeline = new csGL4ForwardPipeline();
      break;
    case eRenderPipeline::eRP_Deferred:
      renderPipeline = new csGL4DeferredPipeline();
      break;
  }
  if (!renderPipeline)
  {
    return nullptr;
  }

  renderPipeline->Initialize();


  csGL4FrameRenderer *frameRenderer = new csGL4FrameRenderer();
  frameRenderer->SetRenderPipeline(renderPipeline);
  return frameRenderer;
}


}
