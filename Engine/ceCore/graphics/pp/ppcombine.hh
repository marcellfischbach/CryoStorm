
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/graphics/postprocessing.hh>

namespace ce
{


CE_CLASS()
class CE_CORE_API PPCombine : public CE_SUPER(SimplePostProcess)
{
  CE_CLASS_GEN;
public:
  PPCombine();
  ~PPCombine() override;

  void Process(ce::iDevice *device) override;

private:
  bool RefreshOutputTexture (ce::iDevice* device);

  ce::iShader *m_shader;
  ce::iShaderAttribute *m_attribColor0;
  ce::iShaderAttribute *m_attribColor1;
};

} // ce
