
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/irenderpipeline.hh>


namespace spc::opengl
{

SPC_CLASS()
class SPC_OGL_API GL4ForwardPipeline : public SPC_SUPER(iRenderPipeline)
{
SPC_CLASS_GEN_OBJECT;
public:
  GL4ForwardPipeline();
  ~GL4ForwardPipeline() override;


  void Render(Camera& camera, Projector& projector, iDevice* device, GfxScene* scene) override;
};

}