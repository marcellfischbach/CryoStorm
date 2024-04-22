

#pragma once

#include <ceCore/graphics/postprocessing.hh>

namespace ce
{
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;
}

CE_CLASS()
class DemoPostProcess : public CE_SUPER(ce::SimplePostProcess)
{
CE_CLASS_GEN_OBJECT;
public:
  DemoPostProcess();

  void Process(ce::iDevice *device, ce::iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (ce::iDevice* device);

  ce::iShader *m_shader;
  ce::iShaderAttribute *m_attribColor;
};