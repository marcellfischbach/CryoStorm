#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/graphics/iframerenderer.hh>

namespace ce
{

struct iRenderPipeline;

CE_CLASS()
class DefaultFrameRenderer : public CE_SUPER(iFrameRenderer)
{
CE_CLASS_GEN_OBJECT;
public:
  DefaultFrameRenderer();
  ~DefaultFrameRenderer() override = default;


  void Render(iRenderTarget2D *target, iDevice *device, iGfxScene *scene) override;
  void Render(iRenderTarget2D *target, GfxCamera *camera, iDevice *device, iGfxScene *scene) override;

private:
  iRenderPipeline *GetRenderPipeline();

  iRenderPipeline *m_renderPipeline;

};


}

