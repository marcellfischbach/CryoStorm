

#pragma once

#include <csCore/graphics/csPostProcessing.hh>

namespace cryo
{
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;
}

CS_CLASS()
class DemoPostProcess : public CS_SUPER(cryo::csSimplePostProcess)
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