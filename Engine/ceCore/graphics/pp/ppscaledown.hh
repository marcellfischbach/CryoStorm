
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace ce
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
  bool RefreshOutputTexture (ce::iDevice* device);

  ce::iShader *m_shader;
  ce::iShaderAttribute *m_attribColor;
  ce::iShaderAttribute *m_attribTextureSizeInv;
};

} // ce
