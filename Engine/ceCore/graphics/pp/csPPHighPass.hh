
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/csPostProcessing.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API csPPHighPass : public CS_SUPER(csSimplePostProcess)
{
  CS_CLASS_GEN;
public:
  csPPHighPass(float highValue = 0.8f);
  ~csPPHighPass() override;

  void Process(iDevice *device, iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (cryo::iDevice* device, iRenderTarget2D *finalTarget);
  float m_highValue;

  cryo::iShader *m_shader;
  cryo::iShaderAttribute *m_attribColor;
  cryo::iShaderAttribute *m_attribHighValue;
};

} // ce
