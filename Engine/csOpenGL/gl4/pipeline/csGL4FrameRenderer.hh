#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/csRef.hh>
#include <csCore/graphics/iFrameRenderer.hh>
#include <csCore/graphics/iRenderPipeline.hh>


namespace cs::opengl
{


CS_CLASS()
class csGL4FrameRenderer : public CS_SUPER(iFrameRenderer)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4FrameRenderer();
  ~csGL4FrameRenderer() override;

  void SetRenderPipeline(iRenderPipeline *renderPipeline);
  void Render(iRenderTarget2D *target, iDevice *device, iGfxScene *scene) override;
  void Render(iRenderTarget2D *target, csGfxCamera *camera, iDevice *device, iGfxScene *scene) override;

private:

  csRef<iRenderPipeline> m_renderPipeline;

};



CS_CLASS()
class csGL4FrameRendererFactory : public CS_SUPER(iFrameRendererFactory)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGL4FrameRendererFactory() = default;
  ~csGL4FrameRendererFactory() override = default;

  csOwned<iFrameRenderer> Create (eRenderPipeline pipeline) const override;
};

}

