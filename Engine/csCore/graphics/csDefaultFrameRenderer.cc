
#include <csCore/graphics/csDefaultFrameRenderer.hh>
#include <csCore/graphics/iRenderPipeline.hh>
#include <csCore/graphics/iRenderTarget2D.hh>
#include <csCore/graphics/scene/csGfxCamera.hh>
#include <csCore/graphics/scene/iGfxScene.hh>
#include <csCore/csObjectRegistry.hh>
#include <csCore/graphics/iDevice.hh>
#include <csCore/graphics/eClearMode.hh>
#include <algorithm>
#include <ranges>

namespace cryo
{


csDefaultFrameRenderer::csDefaultFrameRenderer()
    : m_renderPipeline(nullptr)
{
  CS_CLASS_GEN_CONSTR;
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


void csDefaultFrameRenderer::Render(iRenderTarget2D *target, iDevice *device, iGfxScene *scene)
{
  auto                       cameras = std::vector<csGfxCamera *>(scene->GetCameras());
  std::vector<csGfxCamera *> plainCameras;
  plainCameras.reserve(cameras.size());

  for (auto camera: cameras)
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

void csDefaultFrameRenderer::Render(iRenderTarget2D *target, csGfxCamera *camera, iDevice *device, iGfxScene *scene)
{
  iRenderPipeline *renderPipeline = GetRenderPipeline();
  if (!renderPipeline)
  {
    return;
  }


  camera->UpdateProjector(target->GetWidth(), target->GetHeight());
  renderPipeline->Render(target, camera, device, scene);
}

iRenderPipeline *csDefaultFrameRenderer::GetRenderPipeline()
{
  if (!m_renderPipeline)
  {
    m_renderPipeline = csObjectRegistry::Get<iRenderPipeline>();
  }
  return m_renderPipeline;
}

void csDefaultFrameRenderer::SetRenderPipeline(iRenderPipeline *renderPipeline)
{
  CS_SET(m_renderPipeline, renderPipeline);
}


}
