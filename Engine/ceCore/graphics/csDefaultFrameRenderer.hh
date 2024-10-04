#pragma once

#include <ceCore/csCoreExport.hh>
#include <ceCore/csClass.hh>
#include <ceCore/graphics/iFrameRenderer.hh>

namespace cryo
{

struct iRenderPipeline;

CS_CLASS()
class csDefaultFrameRenderer : public CS_SUPER(iFrameRenderer)
{
CS_CLASS_GEN_OBJECT;
public:
  csDefaultFrameRenderer();
  ~csDefaultFrameRenderer() override = default;

  void SetRenderPipeline(iRenderPipeline *renderPipeline) override;
  void Render(iRenderTarget2D *target, iDevice *device, iGfxScene *scene) override;
  void Render(iRenderTarget2D *target, csGfxCamera *camera, iDevice *device, iGfxScene *scene) override;

private:
  iRenderPipeline *GetRenderPipeline();

  iRenderPipeline *m_renderPipeline;

};


}

