#pragma once

namespace ce
{


struct iDevice;
struct iShader;
struct iShaderAttribute;

namespace opengl
{


class GL4DeferredDirectionalLightRenderer
{
public:

  void Initialize(Settings &settings);
  
private:
  iShader          *m_shader               = nullptr;
  iShaderAttribute *m_attrDiffuseRoughness = nullptr;
  iShaderAttribute *m_attrNormal           = nullptr;
  iShaderAttribute *m_attrDepth            = nullptr;
};

}
}



