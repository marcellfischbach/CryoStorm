
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace ce
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
  bool RefreshOutputTexture (ce::iDevice* device);

  size_t m_sampleCount;
  float m_sampleScale;

  ce::iShader *m_shader;
  ce::iShaderAttribute *m_attribColor;
  ce::iShaderAttribute *m_attribTextureSizeInv;
  ce::iShaderAttribute *m_attribSampleCount;
  ce::iShaderAttribute *m_attribSampleScale;
};

} // ce
