
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/csPostProcessing.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API csPPCombine : public CS_SUPER(csSimplePostProcess)
{
  CS_CLASS_GEN;
public:
  csPPCombine();
  ~csPPCombine() override;

  void Process(iDevice *device, iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture(cryo::iDevice *device, iRenderTarget2D *finalTarget);

  cryo::iShader *m_shader;
  cryo::iShaderAttribute *m_attribColor0;
  cryo::iShaderAttribute *m_attribColor1;
};

} // ce
