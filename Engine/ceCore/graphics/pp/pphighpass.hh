
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace ce
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
  bool RefreshOutputTexture (ce::iDevice* device, iRenderTarget2D *finalTarget);
  float m_highValue;

  ce::iShader *m_shader;
  ce::iShaderAttribute *m_attribColor;
  ce::iShaderAttribute *m_attribHighValue;
};

} // ce
