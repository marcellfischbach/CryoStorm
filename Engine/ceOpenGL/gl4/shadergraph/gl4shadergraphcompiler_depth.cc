

#include <ceOpenGL/gl4/shadergraph/gl4shadergraphcompiler.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/textfile.hh>
#include <ceCore/graphics/evertexstream.hh>
#include <ceCore/graphics/shadergraph/sgnodes.hh>

namespace ce::opengl
{
std::string stream_name(eVertexStream stream);
std::string get_gl_type(eSGValueType type);


std::string GL4ShaderGraphCompiler::GenerateDepth_Vert(std::map<std::string, eMaterialAttributeType> &attributes)
{
  std::string                src;
  std::vector<SGNodeInput *> inputs;
  inputs.push_back(m_shaderGraph->GetDiffuseInput());
  std::vector<SGNode *>    nodes   = ScanNeededVariables(inputs);
  std::vector<StreamInput> streams = FindStreams(nodes);

  std::vector<SGNode *>      noInput;
  std::vector<ResourceInput> resources = FindResources(noInput);

  if (!CollectAttributes(nodes, attributes))
  {
    return "";
  }


  src += "#version 330\n";
  src += "\n";
  src += "layout (location = " + std::to_string(eVS_Vertices) + ") in vec4 cs_Position;\n";

  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices)
    {
      src += "layout (location = " + std::to_string(stream.Stream) + ") " +
             "in " + get_gl_type(stream.Type) + " cs_" + stream_name(stream.Stream) + ";\n";
    }
  }
  src += "\n";

  src += "uniform mat4 cs_ModelViewProjectionMatrix;\n";
  for (const auto &resource: resources)
  {
    src += "uniform " + resource.Type + " cs_" + resource.Name + ";\n";
  }

  src += "\n";
  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices)
    {
      src += "out " + get_gl_type(stream.Type) + " cs_vs_out_" + stream_name(stream.Stream) + ";\n";
    }
  }
  src += "\n";
  src += "void main ()\n";
  src += "{\n";
  src += "  gl_Position = cs_ModelViewProjectionMatrix * cs_Position;\n";

  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices)
    {
      src += "  cs_vs_out_" + stream_name(stream.Stream) + " = cs_" + stream_name(stream.Stream) + ";\n";
    }
  }
  src += "}\n";


  return src;
}

std::string GL4ShaderGraphCompiler::GenerateDepth_Geom(std::map<std::string, eMaterialAttributeType> &attributes)
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

std::string GL4ShaderGraphCompiler::GenerateDepth_Frag(std::map<std::string, eMaterialAttributeType> &attributes)
{
  std::vector<SGNodeInput *> inputs;
  bool                       needAlphaDiscard = m_shaderGraph->GetAlphaDiscard_Func() != eCF_Always &&
                                                m_shaderGraph->GetAlphaDiscard_Func() != eCF_Never;
  if (needAlphaDiscard)
  {
    inputs.push_back(m_shaderGraph->GetAlphaInput());
  }
  std::vector<SGNode *>      nodes     = ScanNeededVariables(inputs);
  std::vector<StreamInput>   streams   = FindStreams(nodes);
  std::vector<ResourceInput> resources = FindResources(nodes);

  if (!CollectAttributes(nodes, attributes))
  {
    return "";
  }

  std::string src;


  src += "#version 330\n";
  src += "layout (location = 0) out vec4 cs_FragColor;\n";
  src += "\n";
  for (const auto &resource: resources)
  {
    src += "uniform " + resource.Type + " cs_" + resource.Name + ";\n";
  }
  src += "\n";
  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices)
    {
      src += "in " + get_gl_type(stream.Type) + " cs_vs_out_" + stream_name(stream.Stream) + ";\n";
    }
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

  for (auto node: nodes)
  {
    const std::string &decl = m_nodeVariables[node].StagedDecl("cs_vs_out_");
    if (!decl.empty())
    {
      src += "  " + decl + "\n";
    }
  }


  if (needAlphaDiscard)
  {
    // calculate the alpha value
    auto        alpha = GetInputValue(m_shaderGraph->GetAlphaInput());
    std::string diffuseVar;
    if (alpha.Type != eSGValueType::Float)
    {
      m_errorString = "Alpha value in depth shader is not float.";
      return "";
    }


    src += "  if (" + alpha.FullQualified() + " " + CompareOperator[m_shaderGraph->GetAlphaDiscard_Func()] + " " +
           std::to_string(m_shaderGraph->GetAlphaDiscard_Threshold()) + ")\n";
    src += "  {\n";
    src += "    discard;\n";
    src += "  }\n";
  }

  src += "  cs_FragColor = vec4(1, 1, 1, 1);\n";
  src += "}\n";

  return src;

}


void GL4ShaderGraphCompiler::GenerateDepth(GL4ShaderGraphCompiler::SourceBundle &bundle)
{
  if (m_shaderGraph->GetQueue() == eRenderQueue::Transparency)
  {
    // no depth rendering in transparent queue
    return;
  }

  bundle.vert = GenerateDepth_Vert(bundle.attributes);
  if (!m_errorString.empty())return;
  bundle.geom = GenerateDepth_Geom(bundle.attributes);
  if (!m_errorString.empty())return;
  bundle.frag = GenerateDepth_Frag(bundle.attributes);
  if (!m_errorString.empty())return;
}


}