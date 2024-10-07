#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/graphics/iFrameRenderer.hh>

namespace cs
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

