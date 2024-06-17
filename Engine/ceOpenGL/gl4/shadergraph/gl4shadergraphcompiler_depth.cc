

#include <ceOpenGL/gl4/shadergraph/gl4shadergraphcompiler.hh>
#include <ceCore/graphics/evertexstream.hh>

namespace ce::opengl
{



std::string GL4ShaderGraphCompiler::GenerateDepth_Vert()
{
  std::string src;



  bool needUVs = m_shaderGraph->GetAlphaDiscard_Func() != eCF_Always && m_shaderGraph->GetAlphaDiscard_Func() != eCF_Never;

  src += "#version 330\n";
  src += "\n";
  src += "layout (location = " + std::to_string(eVS_Vertices) + ") in vec4 ce_Position;\n";

  src += "\n";
  src += "uniform mat4 ce_ModelViewProjectionMatrix;\n";
  src += "\n";
  src += "\n";
  src += "void main ()\n";
  src += "{\n";
  src += "  gl_Position = ce_ModelViewProjectionMatrix * ce_Position;\n";
  src += "}\n";


  return src;
}

std::string GL4ShaderGraphCompiler::GenerateDepth_Geom()
{
  std::string src;


  return src;

}

static std::string CompareOperator[] = {
  std::string("<"),
  std::string("<="),
  std::string(">"),
  std::string(">="),
  std::string("=="),
  std::string("!="),
  std::string(""),
  std::string(""),
};

std::string GL4ShaderGraphCompiler::GenerateDepth_Frag()
{
  std::vector<SGNodeInput*> inputs;
  inputs.push_back(m_shaderGraph->GetDiffuseInput());
  std::vector<SGNode*> nodes = ScanNeededVariables(inputs);




  std::string src;
  bool needAlphaDiscard = m_shaderGraph->GetAlphaDiscard_Func() != eCF_Always && m_shaderGraph->GetAlphaDiscard_Func() != eCF_Never;

  src += "#version 330\n";
  src += "layout (location = 0) out vec4 ce_FragColor;\n";
  src += "\n";
  if (needAlphaDiscard)
  {
    src += "in vec2 outVS_TexCoord;\n";
  }
  src += "\n";
  src += "void main ()\n";
  src += "{\n";
  if (m_shaderGraph->GetAlphaDiscard_Func() == eCF_Always)
  {
    // special case if we discard always, there is not much need to go any further
    src += "  discard;\n";
    src += "}\n";
    return src;
  }

  for (auto node : nodes)
  {
    src += "  " + m_nodeVariables[node].Decl + "\n";
  }

  // calculte the diffuse value
  auto diffuse = GetInputValue(m_shaderGraph->GetDiffuseInput());
  std::string diffuseVar;
  if (diffuse.Type == eSGValueType::Float)
  {
    diffuseVar = "diffuseValue";
    src += "  vec4 diffuseValue = vec4(" + diffuse.Name + ", " + diffuse.Name + ", " + diffuse.Name + ", 1.0);\n";
  }
  else if (diffuse.Type == eSGValueType::Vector3)
  {
    diffuseVar = "diffuseValue";
    src += "  vec4 diffuseValue = vec4(" + diffuse.Name + ", 1.0);\n";
  }
  else if (diffuse.Type == eSGValueType::Vector4)
  {
    diffuseVar = diffuse.Name;
  }

  
  if (needAlphaDiscard)
  {
    src += "  if (" + diffuseVar + ".a " + CompareOperator[m_shaderGraph->GetAlphaDiscard_Func()] + " " + std::to_string(m_shaderGraph->GetAlphaDiscard_Threshold()) + ")\n";
    src += "  {\n";
    src += "    discard;\n";
    src += "  }\n";
  }

  src += "  ce_FragColor = "+diffuseVar+"; \n";
  src += "}\n";

  return src;

}


void GL4ShaderGraphCompiler::GenerateDepth(GL4ShaderGraphCompiler::SourceBundle& bundle)
{

  bundle.vert = GenerateDepth_Vert();
  bundle.geom = GenerateDepth_Geom();
  bundle.frag = GenerateDepth_Frag();
}


}