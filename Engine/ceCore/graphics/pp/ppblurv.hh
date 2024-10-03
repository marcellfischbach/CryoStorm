
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API PPBlurV : public CS_SUPER(SimplePostProcess)
{
  CS_CLASS_GEN;
public:
  PPBlurV(size_t sampleCount = 10, float sampleScale = 1.0f);
  ~PPBlurV() override;

  void Process(iDevice *device, iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (cryo::iDevice* device);

  size_t m_sampleCount;
  float m_sampleScale;

  cryo::iShader *m_shader;
  cryo::iShaderAttribute *m_attribColor;
  cryo::iShaderAttribute *m_attribTextureSizeInv;
  cryo::iShaderAttribute *m_attribSampleCount;
  cryo::iShaderAttribute *m_attribSampleScale;
};

} // ce
