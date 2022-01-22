
#include <spcCore/graphics/defaultframerenderer.hh>
#include <spcCore/graphics/irenderpipeline.hh>
#include <spcCore/graphics/irendertarget2d.hh>
#include <spcCore/graphics/scene/gfxcamera.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/objectregistry.hh>
#include <spcCore/graphics/idevice.hh>
#include <spcCore/graphics/eclearmode.hh>
#include <algorithm>
#include <ranges>

namespace spc
{


DefaultFrameRenderer::DefaultFrameRenderer()
    : m_renderPipeline(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
}

bool gfx_camera_sorter(GfxCamera *cam0, GfxCamera *cam1)
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


void DefaultFrameRenderer::Render(iRenderTarget2D *target, iDevice *device, GfxScene *scene)
{
  auto                     cameras = std::vector<GfxCamera *>(scene->GetCameras());
  std::vector<GfxCamera *> plainCameras;
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

void DefaultFrameRenderer::Render(iRenderTarget2D *target, GfxCamera *camera, iDevice *device, GfxScene *scene)
{
  iRenderPipeline *renderPipeline = GetRenderPipeline();
  if (!renderPipeline)
  {
    return;
  }


  camera->UpdateProjector(target->GetWidth(), target->GetHeight());
  renderPipeline->Render(target, camera, device, scene);
}

iRenderPipeline *DefaultFrameRenderer::GetRenderPipeline()
{
  if (!m_renderPipeline)
  {
    m_renderPipeline = ObjectRegistry::Get<iRenderPipeline>();
  }
  return m_renderPipeline;
}


}
