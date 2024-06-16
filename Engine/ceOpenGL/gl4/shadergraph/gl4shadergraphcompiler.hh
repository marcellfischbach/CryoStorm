//
// Created by Marcell on 15.06.2024.
//

#pragma once

#include <ceOpenGL/openglexport.hh>

#include <ceCore/graphics/shadergraph/ishadergraphcompiler.hh>
#include <ceCore/graphics/shadergraph/sgnode.hh>

namespace ce::opengl
{


CE_CLASS()
class CE_OGL_API GL4ShaderGraphCompiler : public CE_SUPER(iShaderGraphCompiler)
{
  CE_CLASS_GEN_OBJECT;
public:
  GL4ShaderGraphCompiler() = default;


  Material* Compile(ShaderGraph * shaderGraph)  override;

  const std::string& GetError() const override;
private:
  bool CheckForCycle();
  void LinearizeNodes();
  bool Verify();
  bool Verify(SGNode* node);


  std::string m_errorString;
  ShaderGraph* m_shaderGraph;
  std::vector<SGNode*> m_linearizedNodes;

};


CE_CLASS()
class CE_OGL_API GL4ShaderGraphCompilerFactory : public CE_SUPER(iShaderGraphCompilerFactory)
{
  CE_CLASS_GEN_OBJECT;
public:
  GL4ShaderGraphCompilerFactory() = default;

  iShaderGraphCompiler* Create() const override;

};

}
