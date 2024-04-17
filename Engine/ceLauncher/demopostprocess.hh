

#pragma once

#include <ceCore/graphics/postprocessing.hh>

namespace ce
{
struct iRenderTarget2D;
struct iShader;
struct iShaderAttribute;
}

CE_CLASS()
class DemoPostProcess : public CE_SUPER(ce::iPostProcess)
{
CE_CLASS_GEN_OBJECT;
public:
  DemoPostProcess();

  const std::vector<ce::PPInputDefinition> &GetInputDefinitions() const override;
  const std::vector<ce::PPOutputDefinition> &GetOutputDefinitions() const override;
  void SetInput(size_t idx, ce::iTexture2D *texture) override;
  ce::iTexture2D *GetOutput(size_t idx) const override;
  void Process(ce::iDevice *device) override;

private:
  bool RefreshOutputTexture (ce::iDevice* device);

  std::vector<ce::PPInputDefinition> m_inputDefinitions;
  std::vector<ce::PPOutputDefinition> m_outputDefinitions;

  ce::iTexture2D *m_inputTexture = nullptr;
  ce::iRenderTarget2D *m_outputTarget = nullptr;

  ce::iShader *m_shader;
  ce::iShaderAttribute *m_attribColor;
};