
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API PPScaleDown : public CS_SUPER(SimplePostProcess)
{
  CS_CLASS_GEN;
public:
  PPScaleDown();
  ~PPScaleDown() override;

  void Process(iDevice *device, iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (cryo::iDevice* device);

  cryo::iShader *m_shader;
  cryo::iShaderAttribute *m_attribColor;
  cryo::iShaderAttribute *m_attribTextureSizeInv;
};

} // ce
