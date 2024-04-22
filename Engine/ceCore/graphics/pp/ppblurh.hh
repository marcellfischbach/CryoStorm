
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace ce
{


CE_CLASS()
class CE_CORE_API PPBlurH : public CE_SUPER(SimplePostProcess)
{
  CE_CLASS_GEN;
public:
  PPBlurH(size_t sampleCount = 10, float sampleScale = 1.0f);
  ~PPBlurH() override;

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
