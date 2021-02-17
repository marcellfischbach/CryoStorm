
#pragma once

#include <spcOpenGL/openglexport.hh>
#include <spcCore/graphics/irenderpipeline.hh>
#include <spcCore/types.hh>
#include <vector>

namespace spc
{

class GfxLight;


namespace opengl
{

SPC_CLASS()
class SPC_OGL_API GL4ForwardPipeline : public SPC_SUPER(iRenderPipeline)
{
SPC_CLASS_GEN_OBJECT;
public:
  GL4ForwardPipeline();
  ~GL4ForwardPipeline() override;


  void Render(Camera& camera, Projector& projector, iDevice* device, GfxScene* scene) override;

private:
  UInt64 m_frame;

  std::vector<GfxLight*> m_dynamicLights;
  std::vector<GfxLight*> m_staticLights;
  std::vector<GfxLight*> m_staticLightsNew;

};

}

}