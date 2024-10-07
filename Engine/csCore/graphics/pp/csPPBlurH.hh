
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/csPostProcessing.hh>

namespace cs
{


CS_CLASS()
class CS_CORE_API csPPBlurH : public CS_SUPER(csSimplePostProcess)
{
  CS_CLASS_GEN;
public:
  csPPBlurH(size_t sampleCount = 10, float sampleScale = 1.0f);
  ~csPPBlurH() override;

  void Process(iDevice *device, iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (cs::iDevice* device);

  size_t m_sampleCount;
  float m_sampleScale;

  cs::iShader *m_shader;
  cs::iShaderAttribute *m_attribColor;
  cs::iShaderAttribute *m_attribTextureSizeInv;
  cs::iShaderAttribute *m_attribSampleCount;
  cs::iShaderAttribute *m_attribSampleScale;
};

} // ce
