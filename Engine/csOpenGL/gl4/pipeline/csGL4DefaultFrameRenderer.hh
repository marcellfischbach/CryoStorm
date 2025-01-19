#pragma once

#include "csCore/csCoreExport.hh"
#include "csCore/csClass.hh"
#include "csCore/csRef.hh"
#include "csCore/graphics/iFrameRenderer.hh"

namespace cs
{

struct iRenderPipeline;

CS_CLASS()
class csGL4DefaultFrameRenderer : public CS_SUPER(iFrameRenderer)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4DefaultFrameRenderer();
  ~csGL4DefaultFrameRenderer() override = default;

  void SetRenderPipeline(iRenderPipeline *renderPipeline) override;
  void Render(iRenderTarget2D *target, iDevice *device, iGfxScene *scene) override;
  void Render(iRenderTarget2D *target, csGfxCamera *camera, iDevice *device, iGfxScene *scene) override;

private:
  iRenderPipeline *GetRenderPipeline();

  csRef<iRenderPipeline> m_renderPipeline;

};


}

