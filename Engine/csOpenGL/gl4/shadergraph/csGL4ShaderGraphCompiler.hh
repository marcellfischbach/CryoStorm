//
// Created by Marcell on 15.06.2024.
//

#pragma once

#include <csOpenGL/csOpenGLExport.hh>

#include <csCore/graphics/shadergraph/iShaderGraphCompiler.hh>
#include <csCore/graphics/shadergraph/csSGNode.hh>
#include <csCore/graphics/shadergraph/csSGNodes.hh>
#include <csCore/graphics/eVertexStream.hh>


namespace cs::opengl
{


CS_CLASS()
class CS_OGL_API csGL4ShaderGraphCompiler : public CS_SUPER(iShaderGraphCompiler)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4ShaderGraphCompiler() = default;


  csMaterial *Compile(csShaderGraph *shaderGraph, const Parameters &parameters) override;

  const std::string &GetError() const override;


private:
  struct SourceBundle
  {
    std::string                                   vert;
    std::string                                   eval;
    std::string                                   ctrl;
    std::string                                   geom;
    std::string                                   frag;
    std::map<std::string, eMaterialAttributeType> attributes;
  };
  struct NodeVariable
  {
    std::string  Decl;
    std::string  Name;
    eSGValueType Type;
    bool         Stream;

    std::string StagedDecl(const std::string &stage);
  };

  struct OutputVariable
  {
    std::string  Name;
    std::string  PostFix;
    eSGValueType Type;
    bool         Stream;

    std::string FullQualified();

    std::string StagedName();
  };

  struct StreamInput
  {
    eVertexStream Stream;
    eSGValueType  Type;
  };

  struct ResourceInput
  {
    std::string            Name;
    std::string            Type;
    eMaterialAttributeType MatType;
  };

  bool IsNeedingTangent(const std::vector<csSGNode *> &nodes) const;

  bool CollectAttributes(std::vector<csSGNode *> &nodes, std::map<std::string, eMaterialAttributeType> &attributes);

  void GenerateDepth(SourceBundle &bundle);
  std::string GenerateDepth_Vert(std::map<std::string, eMaterialAttributeType> &attributes);
  std::string GenerateDepth_Geom(std::map<std::string, eMaterialAttributeType> &attributes);
  std::string GenerateDepth_Frag(std::map<std::string, eMaterialAttributeType> &attributes);

  void GenerateForward(SourceBundle &bundle);
  std::string GenerateForward_Vert(std::map<std::string, eMaterialAttributeType> &attributes);
  std::string GenerateForward_Geom(std::map<std::string, eMaterialAttributeType> &attributes);
  std::string GenerateForward_Frag(std::map<std::string, eMaterialAttributeType> &attributes);

  void GenerateGBuffer(SourceBundle &bundle);
  std::string GenerateGBuffer_Vert(std::map<std::string, eMaterialAttributeType> &attributes);
  std::string GenerateGBuffer_Geom(std::map<std::string, eMaterialAttributeType> &attributes);
  std::string GenerateGBuffer_Frag(std::map<std::string, eMaterialAttributeType> &attributes);


  bool CheckForCycle();
  void LinearizeNodes();
  bool VerifyNodesType();
  bool VerifyNodeType(csSGNode *node);
  bool VerifyResources();
  void ScanNeededVariables(std::set<csSGNode *> &nodes, csSGNodeInput *input);
  std::vector<csSGNode *> ScanNeededVariables(std::vector<csSGNodeInput *> inputs);
  void AddStream(std::vector<StreamInput> &streams, eVertexStream stream, eSGValueType type);
  std::vector<StreamInput> FindStreams(std::vector<csSGNode *> &nodes);
  void AddResource(std::vector<ResourceInput> &resources,
                   const std::string &resourceName,
                   const std::string &resourceType,
                   eMaterialAttributeType matType);
  std::vector<ResourceInput> FindResources(std::vector<csSGNode *> &nodes);

  void GenerateVariables();
  void GenerateVariable(csSGNode *node);
  void GenerateVariable(csSGNodeOutput *output);
  std::string VarName();
  OutputVariable GetInputValue(csSGNodeInput *input);

  iShader *Compile(SourceBundle &bundle);
  void SetMaterialDefaults(csMaterial* material);



  std::string             m_errorString;
  csShaderGraph           *m_shaderGraph;
  std::vector<csSGNode *> m_linearizedNodes;

  size_t                                     m_nextVariableName;
  std::map<csSGNode *, NodeVariable>         m_nodeVariables;
  std::map<csSGNodeOutput *, OutputVariable> m_outputVariables;
  std::vector<ResourceInput>                 m_resources;

  Parameters m_parameters;

};

class csGL4ShaderGraphLightData
{
public:
  static const csGL4ShaderGraphLightData &Get();
private:
  csGL4ShaderGraphLightData();

public:
  bool        Valid;
  std::string DiffuseLightingDefault;
  std::string DiffuseLightingAttenuated;

  std::string DiffuseLightingAmbient;
  std::string DiffuseLightingDiffuse;
  std::string DiffuseLightingSpecular;
  std::string DiffuseLightingShadowMap;
  std::string DiffuseLightingShadowInline;

};


CS_CLASS()
class CS_OGL_API csGL4ShaderGraphCompilerFactory : public CS_SUPER(iShaderGraphCompilerFactory)
{
CS_CLASS_GEN_OBJECT;
public:
  csGL4ShaderGraphCompilerFactory() = default;

  iShaderGraphCompiler *Create() const override;

};


}
