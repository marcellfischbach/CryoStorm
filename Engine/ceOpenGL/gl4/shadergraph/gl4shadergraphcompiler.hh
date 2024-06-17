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
  struct SourceBundle
  {
    std::string vert;
    std::string eval;
    std::string ctrl;
    std::string geom;
    std::string frag;
  };
  struct NodeVariable
  {
    std::string Decl;
    std::string Name;
    eSGValueType Type;
  };

  struct OutputVariable
  {
    std::string Name;
    std::string PostFix;
    std::string FullQuallified;
    eSGValueType Type;
    eSGValueType FullQuallifiedType;
  };

  void GenerateDepth(SourceBundle& bundle);
  std::string GenerateDepth_Vert();
  std::string GenerateDepth_Geom();
  std::string GenerateDepth_Frag();

  bool CheckForCycle();
  void LinearizeNodes();
  bool Verify();
  bool Verify(SGNode* node);
  void ScanNeededVariables(std::set<SGNode*>& nodes, SGNodeInput* input);
  std::vector<SGNode*> ScanNeededVariables(std::vector<SGNodeInput*> inputs);
  void GenerateVariables();
  void GenerateVariable(SGNode* node);
  void GenerateVariable(SGNodeOutput* output);
  std::string VarName();
  OutputVariable GetInputValue(SGNodeInput *input);

  iShader* Compile(SourceBundle& bundle);


  std::string m_errorString;
  ShaderGraph* m_shaderGraph;
  std::vector<SGNode*> m_linearizedNodes;

  size_t m_nextVariableName;
  std::map<SGNode*, NodeVariable> m_nodeVariables;
  std::map<SGNodeOutput*, OutputVariable> m_outputVariables;
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
