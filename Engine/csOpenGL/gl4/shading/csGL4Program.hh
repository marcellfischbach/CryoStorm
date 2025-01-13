#pragma once

#include <csOpenGL/csOpenGLExport.hh>
#include <csCore/graphics/shading/iShader.hh>
#include <csCore/csTypes.hh>
#include <csCore/csRef.hh>
#include <vector>

namespace cs::opengl
{

class csGL4Shader;

CS_CLASS()
class CS_OGL_API csGL4Program : public CS_SUPER(iShader)
{
CS_CLASS_GEN_OBJECT;
CS_ASSET_GEN;
public:
  csGL4Program();
  virtual ~csGL4Program();

  void AttachShader(csGL4Shader *shader);
  void DetachShader(csGL4Shader *shader);

  void Link();

  uint32_t GetName() const;

  Size RegisterAttribute(const std::string &attributeName) override;

  Size GetAttributeId(const std::string &attributeName) override;

  iShaderAttribute *GetShaderAttribute(uint32_t id) override;

  iShaderAttribute *GetShaderAttribute(const std::string &attributeName) override;

  iShaderAttribute *GetShaderAttribute(eShaderAttribute shaderAttribute) override;


private:
  void RegisterRenderAttributes();

  uint32_t m_name;

  std::vector<csAssetRef<csGL4Shader>> m_shaders;

  std::vector<iShaderAttribute *> m_attributes;

};

}

