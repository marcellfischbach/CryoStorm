
#include <spcCore/graphics/defaultframerenderer.hh>
#include <spcCore/graphics/irenderpipeline.hh>
#include <spcCore/graphics/irendertarget2d.hh>
#include <spcCore/graphics/scene/gfxcamera.hh>
#include <spcCore/graphics/scene/gfxscene.hh>
#include <spcCore/objectregistry.hh>

namespace spc
{


DefaultFrameRenderer::DefaultFrameRenderer()
    : m_renderPipeline(nullptr)
{
  SPC_CLASS_GEN_CONSTR;
}


void DefaultFrameRenderer::Render(iRenderTarget2D *target, iDevice *device, GfxScene *scene)
{
  iRenderPipeline *renderPipeline = GetRenderPipeline();
  if (!renderPipeline)
  {
    return;
  }

  auto cameras = scene->GetCameras();
  for (auto camera : cameras)
  {
    camera->UpdateProjector(target->GetWidth(), target->GetHeight());

    renderPipeline->Render(target, *camera->GetCamera(), *camera->GetProjector(), device, scene);
  }
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
