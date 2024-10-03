
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API PPHighPass : public CS_SUPER(SimplePostProcess)
{
  CS_CLASS_GEN;
public:
  PPHighPass(float highValue = 0.8f);
  ~PPHighPass() override;

  void Process(iDevice *device, iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (cryo::iDevice* device, iRenderTarget2D *finalTarget);
  float m_highValue;

  cryo::iShader *m_shader;
  cryo::iShaderAttribute *m_attribColor;
  cryo::iShaderAttribute *m_attribHighValue;
};

} // ce
