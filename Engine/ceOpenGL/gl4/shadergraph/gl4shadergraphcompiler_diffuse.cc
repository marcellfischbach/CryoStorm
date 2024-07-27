

#include <ceOpenGL/gl4/shadergraph/gl4shadergraphcompiler.hh>
#include <ceCore/resource/assetmanager.hh>
#include <ceCore/resource/textfile.hh>
#include <ceCore/graphics/evertexstream.hh>
#include <ceCore/graphics/shadergraph/sgnodes.hh>

namespace ce::opengl
{
std::string stream_name(eVertexStream stream);
std::string get_gl_type(eSGValueType type);

std::string GL4ShaderGraphCompiler::GenerateForward_Vert(std::map<std::string, eMaterialAttributeType> &attributes)
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

  bool tangentsNeeded = hasNormalInput || IsNeedingTangent(nodes);


  std::vector<SGNode *>      noInput;
  std::vector<ResourceInput> resources = FindResources(noInput);

  src += R"(
#version 330
layout (location = )" + std::to_string(eVS_Vertices) + R"() in vec4 ce_Position;
layout (location = )" + std::to_string(eVS_Normals) + R"() in vec3 ce_Normal;
)";

  if (tangentsNeeded)
  {
    src += "layout (location = " + std::to_string(eVS_Tangents) + ") in vec3 ce_Tangent;\n";
  }


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
uniform mat4 ce_ViewMatrix;
uniform mat4 ce_ViewMatrixInv;
uniform mat4 ce_ViewProjectionMatrix;
uniform mat4 ce_ModelViewProjectionMatrix;
)";
  for (const auto &resource: resources)
  {
    src += "uniform " + resource.Type + " ce_" + resource.Name + ";\n";
  }

  src += R"(
out vec3 ce_vs_out_WorldPosition;
out vec3 ce_vs_out_WorldNormal;
)";
  if (tangentsNeeded)
  {
    src += "out vec3 ce_vs_out_WorldTangent;\n";
  }
  src += R"(
out vec3 ce_vs_out_CameraWorldPosition;
out vec3 ce_vs_out_CameraSpacePosition;
out vec2 ce_vs_out_ScreenCoordinates;
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
  vec4 position = ce_ModelMatrix * ce_Position;
  ce_vs_out_WorldPosition = position.xyz;
  ce_vs_out_WorldNormal = mat3(ce_ModelMatrix) * ce_Normal;
)";
  if (tangentsNeeded)
  {
    src += "  ce_vs_out_WorldTangent = mat3(ce_ModelMatrix) * ce_Tangent;\n";
  }
  src += R"(

  ce_vs_out_CameraWorldPosition = (ce_ViewMatrixInv * vec4(0, 0, 0, 1)).xyz;
  ce_vs_out_CameraSpacePosition = (ce_ViewMatrix * position).xyz;

  gl_Position = ce_ModelViewProjectionMatrix * ce_Position;
  ce_vs_out_ScreenCoordinates = (gl_Position.xy / gl_Position.w) * 0.5 + 0.5;
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

std::string GL4ShaderGraphCompiler::GenerateForward_Geom(std::map<std::string, eMaterialAttributeType> &attributes)
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

std::string GL4ShaderGraphCompiler::GenerateForward_Frag(std::map<std::string, eMaterialAttributeType> &attributes)
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
layout (location = 0) out vec4 ce_FragColor;
)";
  src += "\n";
  for (const auto &resource: resources)
  {
    src += "uniform " + resource.Type + " ce_" + resource.Name + ";\n";
  }
  src += R"(
in vec3 ce_vs_out_WorldPosition;
in vec3 ce_vs_out_WorldNormal;
)";
  if (tangentsNeeded)
  {
    src += "in vec3 ce_vs_out_WorldTangent;\n";
  }

  src += R"(
in vec3 ce_vs_out_CameraWorldPosition;
in vec3 ce_vs_out_CameraSpacePosition;
in vec2 ce_vs_out_ScreenCoordinates;
)";

  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices && stream.Stream != eVS_Normals && stream.Stream != eVS_Tangents)
    {
      src += "in " + get_gl_type(stream.Type) + " ce_vs_out_" + stream_name(stream.Stream) + ";\n";
    }
  }


  if (m_shaderGraph->GetLightingMode() == ShaderGraph::eLM_Default)
  {
    src += GL4ShaderGraphLightData::Get().DiffuseLightingDefault;
    src += GL4ShaderGraphLightData::Get().DiffuseLightingAmbient;
    src += GL4ShaderGraphLightData::Get().DiffuseLightingDiffuse;
    src += GL4ShaderGraphLightData::Get().DiffuseLightingSpecular;
  }
  else if(m_shaderGraph->GetLightingMode() == ShaderGraph::eLM_Attenuated)
  {
    src += GL4ShaderGraphLightData::Get().DiffuseLightingAttenuated;
  }

  if (m_shaderGraph->GetQueue() == eRenderQueue::Transparency)
  {
    src += GL4ShaderGraphLightData::Get().DiffuseLightingShadowInline;
  }
  else
  {
    src += GL4ShaderGraphLightData::Get().DiffuseLightingShadowMap;
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
      m_errorString = "Alpha value in depth shader is not float.";
      return "";
    }

    src += "  if (" + alpha.FullQualified() + " " + CompareOperator[m_shaderGraph->GetAlphaDiscard_Func()] + " " +
           std::to_string(m_shaderGraph->GetAlphaDiscard_Threshold()) + ")\n";
    src += "  {\n";
    src += "    discard;\n";
    src += "  }\n";
  }

  if (m_shaderGraph->GetLightingMode() == ShaderGraph::eLM_Default)
  {
    src += R"(
  vec3 norm = normalize (ce_vs_out_WorldNormal);
)";
    if (tangentsNeeded || hasNormalInput)
    {
      src += R"(
  vec3 tang = normalize (ce_vs_out_WorldTangent);
  vec3 binormal = normalize (cross(norm, tang));
  tang = cross(binormal, norm);

  mat3 normalMatrix = mat3(tang, binormal, norm);
)";
    }
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
  vec3 normal = )" + normal.FullQualified() + R"(;
  normal = normal * 2.0 - 1.0;
  normal = normalMatrix * normal;
)";
  }
  else
  {
    src += R"(  vec3 normal = norm;
)";
  }

  auto diffuse   = GetInputValue(m_shaderGraph->GetDiffuseInput());
  if (m_shaderGraph->GetLightingMode() == ShaderGraph::eLM_Default)
  {
    auto roughness = GetInputValue(m_shaderGraph->GetRoughnessInput());
    auto alpha     = GetInputValue(m_shaderGraph->GetAlphaInput());
    auto metallic  = GetInputValue(m_shaderGraph->GetMetallicInput());


    src += "  \n";
    src += "  float roughness = " + roughness.FullQualified() + ";\n";
    src += "  vec3 diffuse = " + diffuse.FullQualified() + ";\n";
    src += "  float alpha = " + alpha.FullQualified() + ";\n";
    src += "  float metallic = " + metallic.FullQualified() + ";\n";

    src += R"(

  light_result_t light = calc_lights(ce_vs_out_WorldPosition, normal, ce_vs_out_CameraSpacePosition, ce_vs_out_CameraWorldPosition, 0.8, roughness);
  vec3 dielectric_light = light.diffuse  * diffuse + light.specular;
  vec3 metallic_light = light.specular * diffuse;

  ce_FragColor = vec4(mix(dielectric_light, metallic_light, metallic), alpha);
)";
  }
  else if (m_shaderGraph->GetLightingMode() == ShaderGraph::eLM_Attenuated)
  {
    src += "  vec3 diffuse = " + diffuse.FullQualified() + ";\n";
    src += R"(

  light_result_t light = calc_lights(ce_vs_out_WorldPosition, vec3(0, 0, 0), ce_vs_out_CameraSpacePosition, ce_vs_out_CameraWorldPosition, 0.0, 0.0);
  vec3 dielectric_light = light.diffuse  * diffuse;

  ce_FragColor = vec4(dielectric_light, alpha);
)";

  }
  else
  {
    src += "  vec3 diffuse = " + diffuse.FullQualified() + ";\n";
    src += "  ce_FragColor = vec4(diffuse, alpha);\n";
  }

  src += R"(
}

)";


  return src;

}


void GL4ShaderGraphCompiler::GenerateForward(GL4ShaderGraphCompiler::SourceBundle &bundle)
{
  bundle.vert = GenerateForward_Vert(bundle.attributes);
  if (m_errorString.empty())
  { return; }
  bundle.geom = GenerateForward_Geom(bundle.attributes);
  if (m_errorString.empty())
  { return; }
  bundle.frag = GenerateForward_Frag(bundle.attributes);
  if (m_errorString.empty())
  { return; }
  bundle.attributes["ReceiveShadow"] = eMAT_Int;
}


}