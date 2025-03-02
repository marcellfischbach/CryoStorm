

#pragma once

#include <csCore/graphics/csPostProcessing.hh>
#include <csCore/csRef.hh>

namespace cs
{
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;
}

CS_CLASS()
class DemoPostProcess : public CS_SUPER(cs::csSimplePostProcess)
{
CS_CLASS_GEN_OBJECT;
public:
  DemoPostProcess();

  void Process(cs::iDevice *device, cs::iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (cs::iDevice* device);

  cs::csAssetRef<cs::iShader> m_shader;
  cs::iShaderAttribute *m_attribColor;
};