

#pragma once

#include <ceCore/graphics/postprocessing.hh>

namespace ce
{
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;
}

CS_CLASS()
class DemoPostProcess : public CS_SUPER(ce::SimplePostProcess)
{
CS_CLASS_GEN_OBJECT;
public:
  DemoPostProcess();

  void Process(ce::iDevice *device, ce::iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (ce::iDevice* device);

  ce::iShader *m_shader;
  ce::iShaderAttribute *m_attribColor;
};