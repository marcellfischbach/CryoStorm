
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace cryo
{


CS_CLASS()
class CS_CORE_API PPCombine : public CS_SUPER(SimplePostProcess)
{
  CS_CLASS_GEN;
public:
  PPCombine();
  ~PPCombine() override;

  void Process(iDevice *device, iRenderTarget2D *finalTarget) override;

private:
  bool RefreshOutputTexture(cryo::iDevice *device, iRenderTarget2D *finalTarget);

  cryo::iShader *m_shader;
  cryo::iShaderAttribute *m_attribColor0;
  cryo::iShaderAttribute *m_attribColor1;
};

} // ce
