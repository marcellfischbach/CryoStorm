
#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/graphics/csPostProcessing.hh>
#include <csCore/csRef.hh>

namespace cs
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
  bool RefreshOutputTexture(cs::iDevice *device, iRenderTarget2D *finalTarget);

  csAssetRef<iShader> m_shader;
  cs::iShaderAttribute *m_attribColor0;
  cs::iShaderAttribute *m_attribColor1;
};

} // ce
