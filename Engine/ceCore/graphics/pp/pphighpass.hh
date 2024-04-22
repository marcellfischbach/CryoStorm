
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace ce
{


CE_CLASS()
class CE_CORE_API PPHighPass : public CE_SUPER(SimplePostProcess)
{
  CE_CLASS_GEN;
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
