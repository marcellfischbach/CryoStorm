

#pragma once

#include <ceCore/graphics/postprocessing.hh>

namespace cryo
{
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;
}

CS_CLASS()
class DemoPostProcess : public CS_SUPER(cryo::SimplePostProcess)
{
CS_CLASS_GEN_OBJECT;
public:
  DemoPostProcess();

  void Process(cryo::iDevice *device, cryo::iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (cryo::iDevice* device);

  cryo::iShader *m_shader;
  cryo::iShaderAttribute *m_attribColor;
};