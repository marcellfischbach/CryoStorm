#pragma once

#include <spcCore/coreexport.hh>
#include <spcCore/class.hh>
#include <spcCore/graphics/iframerenderer.hh>

namespace spc
{

struct iRenderPipeline;

SPC_CLASS()
class DefaultFrameRenderer : public SPC_SUPER(iFrameRenderer)
{
  SPC_CLASS_GEN_OBJECT;
public:
  DefaultFrameRenderer();
  virtual ~DefaultFrameRenderer() = default;


  void Render(iRenderTarget2D *target, iDevice *device, GfxScene *scene) override;

  void Render(iRenderTarget2D *target, GfxCamera *camera, iDevice* device,  GfxScene *scene) override;

private:
  iRenderPipeline *GetRenderPipeline ();

  iRenderPipeline *m_renderPipeline;

};


}

