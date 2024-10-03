#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/iframerenderer.hh>

namespace cryo
{

struct iRenderPipeline;

CS_CLASS()
class DefaultFrameRenderer : public CS_SUPER(iFrameRenderer)
{
CS_CLASS_GEN_OBJECT;
public:
  DefaultFrameRenderer();
  ~DefaultFrameRenderer() override = default;

  void SetRenderPipeline(iRenderPipeline *renderPipeline) override;
  void Render(iRenderTarget2D *target, iDevice *device, iGfxScene *scene) override;
  void Render(iRenderTarget2D *target, GfxCamera *camera, iDevice *device, iGfxScene *scene) override;

private:
  iRenderPipeline *GetRenderPipeline();

  iRenderPipeline *m_renderPipeline;

};


}

