
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/csPostProcessing.hh>

namespace cs
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
  bool RefreshOutputTexture (cs::iDevice* device);

  csAssetRef<iShader> m_shader;
  cs::iShaderAttribute *m_attribColor;
  cs::iShaderAttribute *m_attribTextureSizeInv;
};

} // ce
