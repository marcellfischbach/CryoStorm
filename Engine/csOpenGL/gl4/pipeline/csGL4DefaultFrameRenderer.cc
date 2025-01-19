
#include <csOpenGL/gl4/pipeline/csGL4DefaultFrameRenderer.hh>
#include <csCore/graphics/iRenderPipeline.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/scene/csGfxCamera.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/eClearMode.hh>
#include <algorithm>
#include <ranges>

namespace cs
{


csGL4DefaultFrameRenderer::csGL4DefaultFrameRenderer()
//    : m_renderPipeline(nullptr)
{

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


void csGL4DefaultFrameRenderer::Render(iRenderTarget2D *target, iDevice *device, iGfxScene *scene)
{
  auto cameras = std::vector<csRef<csGfxCamera>>(scene->GetCameras());
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

void csGL4DefaultFrameRenderer::Render(iRenderTarget2D *target, csGfxCamera *camera, iDevice *device, iGfxScene *scene)
{
  iRenderPipeline *renderPipeline = GetRenderPipeline();
  if (!renderPipeline)
  {
    return;
  }


  camera->UpdateProjector(target->GetWidth(), target->GetHeight());
  renderPipeline->Render(target, camera, device, scene);
}

iRenderPipeline *csGL4DefaultFrameRenderer::GetRenderPipeline()
{
  if (!m_renderPipeline)
  {
    m_renderPipeline = csObjectRegistry::Get<iRenderPipeline>();
  }
  return m_renderPipeline;
}

void csGL4DefaultFrameRenderer::SetRenderPipeline(iRenderPipeline *renderPipeline)
{
  m_renderPipeline = renderPipeline;
}


}
