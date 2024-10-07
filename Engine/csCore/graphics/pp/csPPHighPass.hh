
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/csPostProcessing.hh>

namespace cs
{


CS_CLASS()
class CS_CORE_API csPPHighPass : public CS_SUPER(csSimplePostProcess)
{
  CS_CLASS_GEN;
public:
  csPPHighPass(float highValue = 0.8f);
  ~csPPHighPass() override;

  void Process(iDevice *device, iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (cs::iDevice* device, iRenderTarget2D *finalTarget);
  float m_highValue;

  cs::iShader *m_shader;
  cs::iShaderAttribute *m_attribColor;
  cs::iShaderAttribute *m_attribHighValue;
};

} // ce
