

#include <ceOpenGL/gl4/shadergraph/gl4shadergraphcompiler.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/textfile.hh>
#include <ceCore/graphics/evertexstream.hh>
#include <ceCore/graphics/shadergraph/sgnodes.hh>

namespace ce::opengl
{
std::string stream_name(eVertexStream stream);
std::string get_gl_type(eSGValueType type);

std::string GL4ShaderGraphCompiler::GenerateGBuffer_Vert(std::map<std::string, eMaterialAttributeType> &attributes)
{
  std::string src;

  std::vector<SGNodeInput *> inputs;
  inputs.push_back(m_shaderGraph->GetDiffuseInput());
  inputs.push_back(m_shaderGraph->GetAlphaInput());
  inputs.push_back(m_shaderGraph->GetRoughnessInput());
  inputs.push_back(m_shaderGraph->GetNormalInput());
  std::vector<SGNode *>    nodes   = ScanNeededVariables(inputs);
  std::vector<StreamInput> streams = FindStreams(nodes);

  if (!CollectAttributes(nodes, attributes))
  {
    return "";
  }

  bool hasNormalInput = m_shaderGraph->GetNormalInput()->GetSource() != nullptr;


  std::vector<SGNode *>      noInput;
  std::vector<ResourceInput> resources = FindResources(noInput);

  src += R"(
#version 330
layout (location = )" + std::to_string(eVS_Vertices) + R"() in vec4 ce_Position;
layout (location = )" + std::to_string(eVS_Normals) + R"() in vec3 ce_Normal;
layout (location = )" + std::to_string(eVS_Tangents) + R"() in vec3 ce_Tangent;
)";


  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices && stream.Stream != eVS_Normals && stream.Stream != eVS_Tangents)
    {
      src += "layout (location = " + std::to_string(stream.Stream) + ") " +
             "in " + get_gl_type(stream.Type) + " ce_" + stream_name(stream.Stream) + ";\n";
    }
  }
  src += R"(

uniform mat4 ce_ModelMatrix;
uniform mat4 ce_ModelViewProjectionMatrix;
)";
  for (const auto &resource: resources)
  {
    src += "uniform " + resource.Type + " ce_" + resource.Name + ";\n";
  }

  src += R"(
out vec3 ce_vs_out_WorldNormal;
out vec3 ce_vs_out_WorldTangent;
)";

  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices && stream.Stream != eVS_Normals && stream.Stream != eVS_Tangents)
    {
      src += "out " + get_gl_type(stream.Type) + " ce_vs_out_" + stream_name(stream.Stream) + ";\n";
    }
  }

  src += R"(
void main ()
{
  ce_vs_out_WorldNormal = mat3(ce_ModelMatrix) * ce_Normal;
  ce_vs_out_WorldTangent = mat3(ce_ModelMatrix) * ce_Tangent;
  gl_Position = ce_ModelViewProjectionMatrix * ce_Position;
  )";
  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices && stream.Stream != eVS_Normals && stream.Stream != eVS_Tangents)
    {
      src += "  ce_vs_out_" + stream_name(stream.Stream) + " = ce_" + stream_name(stream.Stream) + ";\n";
    }
  }
  src += "}\n\n";


  return src;
}

std::string GL4ShaderGraphCompiler::GenerateGBuffer_Geom(std::map<std::string, eMaterialAttributeType> &attributes)
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

std::string GL4ShaderGraphCompiler::GenerateGBuffer_Frag(std::map<std::string, eMaterialAttributeType> &attributes)
{
  std::string src;

  std::vector<SGNodeInput *> inputs;
  inputs.push_back(m_shaderGraph->GetDiffuseInput());
  inputs.push_back(m_shaderGraph->GetAlphaInput());
  inputs.push_back(m_shaderGraph->GetRoughnessInput());
  inputs.push_back(m_shaderGraph->GetNormalInput());
  std::vector<SGNode *> nodes = ScanNeededVariables(inputs);
  if (!CollectAttributes(nodes, attributes))
  {
    return "";
  }

  // collect the alpha nodes separately, because we might make an early discard
  inputs.clear();
  inputs.push_back(m_shaderGraph->GetAlphaInput());
  std::vector<SGNode *> alphaNodes = ScanNeededVariables(inputs);

  std::vector<StreamInput> streams = FindStreams(nodes);


  bool hasNormalInput = m_shaderGraph->GetNormalInput()->GetSource() != nullptr;

  bool tangentsNeeded = hasNormalInput || IsNeedingTangent(nodes);

  bool needAlphaDiscard = m_shaderGraph->GetAlphaDiscard_Func() != eCF_Always &&
                          m_shaderGraph->GetAlphaDiscard_Func() != eCF_Never;


  std::vector<SGNode *>      noInput;
  std::vector<ResourceInput> resources = FindResources(nodes);
  src += R"(

#version 330
layout(location = 0) out vec4 ce_FragDiffuseRoughness;
layout(location = 1) out vec4 ce_FragNormal;
layout(location = 2) out vec4 ce_FragEmission;

)";
  src += "\n";
  for (const auto &resource: resources)
  {
    src += "uniform " + resource.Type + " ce_" + resource.Name + ";\n";
  }
  src += R"(
in vec3 ce_vs_out_WorldNormal;
in vec3 ce_vs_out_WorldTangent;
)";

  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices && stream.Stream != eVS_Normals && stream.Stream != eVS_Tangents)
    {
      src += "in " + get_gl_type(stream.Type) + " ce_vs_out_" + stream_name(stream.Stream) + ";\n";
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

  for (auto node: alphaNodes)
  {
    const std::string &decl = m_nodeVariables[node].StagedDecl("ce_vs_out_");
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
      m_errorString = "Alpha value in gbuffer shader is not float.";
      return "";
    }

    src += "  if (" + alpha.FullQualified() + " " + CompareOperator[m_shaderGraph->GetAlphaDiscard_Func()] + " " +
           std::to_string(m_shaderGraph->GetAlphaDiscard_Threshold()) + ")\n";
    src += "  {\n";
    src += "    discard;\n";
    src += "  }\n";
  }


  for (auto node: nodes)
  {
    if (std::ranges::find(alphaNodes.begin(), alphaNodes.end(), node) != alphaNodes.end())
    {
      // this node is already processed
      continue;
    }
    const std::string &decl = m_nodeVariables[node].StagedDecl("ce_vs_out_");
    if (!decl.empty())
    {
      src += "  " + decl + "\n";
    }
  }
  if (hasNormalInput)
  {
    auto normal = GetInputValue(m_shaderGraph->GetNormalInput());
    src += R"(
  vec3 norm = normalize (ce_vs_out_WorldNormal);
  vec3 tang = normalize (ce_vs_out_WorldTangent);
  vec3 binormal = normalize (cross(norm, tang));
  tang = cross(binormal, norm);
  mat3 normalMatrix = mat3(tang, binormal, norm);

  vec3 normal = )" + normal.FullQualified() + R"(;
  normal = normal * 2.0 - 1.0;
  normal = normalMatrix * normal;
  ce_FragNormal = vec4(normal * 0.5 + 0.5, 1.0);
)";
  }
  else
  {
    src += R"(
  vec3 norm = normalize (ce_vs_out_WorldNormal);
  ce_FragNormal = vec4(norm * 0.5 + 0.5, 1.0);
)";
  }


  auto diffuse   = GetInputValue(m_shaderGraph->GetDiffuseInput());
  auto roughness = GetInputValue(m_shaderGraph->GetRoughnessInput());
  auto metallic  = GetInputValue(m_shaderGraph->GetMetallicInput());

  src += "  ce_FragDiffuseRoughness = vec4(" + diffuse.FullQualified() + ", " + roughness.FullQualified() + ");\n";
  src += "  ce_FragEmission = vec4(0, 0, 0, 0);\n";

  src += "}\n\n";


  return src;

}


void GL4ShaderGraphCompiler::GenerateGBuffer(GL4ShaderGraphCompiler::SourceBundle &bundle)
{
  bundle.vert = GenerateGBuffer_Vert(bundle.attributes);
  if (!m_errorString.empty())
  { return; }
  bundle.geom = GenerateGBuffer_Geom(bundle.attributes);
  if (!m_errorString.empty())
  { return; }
  bundle.frag = GenerateGBuffer_Frag(bundle.attributes);
  if (!m_errorString.empty())
  { return; }
  bundle.attributes["ReceiveShadow"] = eMAT_Int;
}


}