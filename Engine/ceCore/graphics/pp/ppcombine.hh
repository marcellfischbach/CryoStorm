
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace ce
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
  bool RefreshOutputTexture(ce::iDevice *device, iRenderTarget2D *finalTarget);

  ce::iShader *m_shader;
  ce::iShaderAttribute *m_attribColor0;
  ce::iShaderAttribute *m_attribColor1;
};

} // ce
