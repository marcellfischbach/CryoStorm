
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/csPostProcessing.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API csPPScaleDown : public CS_SUPER(csSimplePostProcess)
{
  CS_CLASS_GEN;
public:
  csPPScaleDown();
  ~csPPScaleDown() override;

  void Process(iDevice *device, iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture (cryo::iDevice* device);

  cryo::iShader *m_shader;
  cryo::iShaderAttribute *m_attribColor;
  cryo::iShaderAttribute *m_attribTextureSizeInv;
};

} // ce
