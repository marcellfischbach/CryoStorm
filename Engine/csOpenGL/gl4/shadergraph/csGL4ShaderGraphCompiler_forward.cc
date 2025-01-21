

#include <csOpenGL/gl4/shadergraph/csGL4ShaderGraphCompiler.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csTextFile.hh>
#include <csCore/graphics/eVertexStream.hh>
#include <csCore/graphics/shadergraph/csSGNodes.hh>

namespace cs::opengl
{
std::string stream_name(eVertexStream stream);
std::string get_gl_type(eSGValueType type);

std::string csGL4ShaderGraphCompiler::GenerateForward_Vert(std::map<std::string, eMaterialAttributeType> &attributes)
{
  std::string src;

  std::vector<csSGNodeInput *> inputs;
  inputs.push_back(m_shaderGraph->GetDiffuseInput());
  inputs.push_back(m_shaderGraph->GetAlphaInput());
  inputs.push_back(m_shaderGraph->GetRoughnessInput());
  inputs.push_back(m_shaderGraph->GetNormalInput());
  std::vector<csSGNode *>  nodes   = ScanNeededVariables(inputs);
  std::vector<StreamInput> streams = FindStreams(nodes);

  if (!CollectAttributes(nodes, attributes))
  {
    return "";
  }

  bool hasNormalInput = m_shaderGraph->GetNormalInput()->GetSource() != nullptr;

  bool tangentsNeeded = hasNormalInput || IsNeedingTangent(nodes);


  std::vector<csSGNode *>    noInput;
  std::vector<ResourceInput> resources = FindResources(noInput);

  src += R"(
#version 330
layout (location = )" + std::to_string(eVS_Vertices) + R"() in vec4 cs_Position;
layout (location = )" + std::to_string(eVS_Normals) + R"() in vec3 cs_Normal;
)";

  if (tangentsNeeded)
  {
    src += "layout (location = " + std::to_string(eVS_Tangents) + ") in vec3 cs_Tangent;\n";
  }


  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices && stream.Stream != eVS_Normals && stream.Stream != eVS_Tangents)
    {
      src += "layout (location = " + std::to_string(stream.Stream) + ") " +
             "in " + get_gl_type(stream.Type) + " cs_" + stream_name(stream.Stream) + ";\n";
    }
  }
  src += R"(

uniform mat4 cs_ModelMatrix;
uniform mat4 cs_ViewMatrix;
uniform mat4 cs_ViewMatrixInv;
uniform mat4 cs_ViewProjectionMatrix;
uniform mat4 cs_ModelViewProjectionMatrix;
)";
  for (const auto &resource: resources)
  {
    src += "uniform " + resource.Type + " cs_" + resource.Name + ";\n";
  }

  src += R"(
out vec3 cs_vs_out_WorldPosition;
out vec3 cs_vs_out_WorldNormal;
)";
  if (tangentsNeeded)
  {
    src += "out vec3 cs_vs_out_WorldTangent;\n";
  }
  src += R"(
out vec3 cs_vs_out_CameraWorldPosition;
out vec3 cs_vs_out_CameraSpacePosition;
out vec2 cs_vs_out_ScreenCoordinates;
)";

  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices && stream.Stream != eVS_Normals && stream.Stream != eVS_Tangents)
    {
      src += "out " + get_gl_type(stream.Type) + " cs_vs_out_" + stream_name(stream.Stream) + ";\n";
    }
  }

  src += R"(
void main ()
{
  vec4 position = cs_ModelMatrix * cs_Position;
  cs_vs_out_WorldPosition = position.xyz;
  cs_vs_out_WorldNormal = mat3(cs_ModelMatrix) * cs_Normal;
)";
  if (tangentsNeeded)
  {
    src += "  cs_vs_out_WorldTangent = mat3(cs_ModelMatrix) * cs_Tangent;\n";
  }
  src += R"(

  cs_vs_out_CameraWorldPosition = (cs_ViewMatrixInv * vec4(0, 0, 0, 1)).xyz;
  cs_vs_out_CameraSpacePosition = (cs_ViewMatrix * position).xyz;

  gl_Position = cs_ModelViewProjectionMatrix * cs_Position;
  cs_vs_out_ScreenCoordinates = (gl_Position.xy / gl_Position.w) * 0.5 + 0.5;
  )";
  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices && stream.Stream != eVS_Normals && stream.Stream != eVS_Tangents)
    {
      src += "  cs_vs_out_" + stream_name(stream.Stream) + " = cs_" + stream_name(stream.Stream) + ";\n";
    }
  }
  src += "}\n\n";


  return src;
}

std::string csGL4ShaderGraphCompiler::GenerateForward_Geom(std::map<std::string, eMaterialAttributeType> &attributes)
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

std::string csGL4ShaderGraphCompiler::GenerateForward_Frag(std::map<std::string, eMaterialAttributeType> &attributes)
{
  std::string src;

  std::vector<csSGNodeInput *> inputs;
  inputs.push_back(m_shaderGraph->GetDiffuseInput());
  inputs.push_back(m_shaderGraph->GetAlphaInput());
  inputs.push_back(m_shaderGraph->GetRoughnessInput());
  inputs.push_back(m_shaderGraph->GetNormalInput());
  inputs.push_back(m_shaderGraph->GetMetallicInput());
  std::vector<csSGNode *> nodes = ScanNeededVariables(inputs);
  if (!CollectAttributes(nodes, attributes))
  {
    return "";
  }

  // collect the alpha nodes separately, because we might make an early discard
  inputs.clear();
  inputs.push_back(m_shaderGraph->GetAlphaInput());
  std::vector<csSGNode *> alphaNodes = ScanNeededVariables(inputs);

  std::vector<StreamInput> streams = FindStreams(nodes);


  bool hasNormalInput = m_shaderGraph->GetNormalInput()->GetSource() != nullptr;

  bool tangentsNeeded = hasNormalInput || IsNeedingTangent(nodes);

  bool needAlphaDiscard = m_shaderGraph->GetAlphaDiscard_Func() != eCF_Always &&
                          m_shaderGraph->GetAlphaDiscard_Func() != eCF_Never;


  std::vector<csSGNode *>    noInput;
  std::vector<ResourceInput> resources = FindResources(nodes);
  src += R"(

#version 330
layout (location = 0) out vec4 cs_FragColor;
)";
  src += "\n";
  for (const auto &resource: resources)
  {
    src += "uniform " + resource.Type + " cs_" + resource.Name + ";\n";
  }
  src += R"(
in vec3 cs_vs_out_WorldPosition;
in vec3 cs_vs_out_WorldNormal;
)";
  if (tangentsNeeded)
  {
    src += "in vec3 cs_vs_out_WorldTangent;\n";
  }

  src += R"(
in vec3 cs_vs_out_CameraWorldPosition;
in vec3 cs_vs_out_CameraSpacePosition;
in vec2 cs_vs_out_ScreenCoordinates;
)";

  for (const auto &stream: streams)
  {
    if (stream.Stream != eVS_Vertices && stream.Stream != eVS_Normals && stream.Stream != eVS_Tangents)
    {
      src += "in " + get_gl_type(stream.Type) + " cs_vs_out_" + stream_name(stream.Stream) + ";\n";
    }
  }


  if (m_shaderGraph->GetLightingMode() == csShaderGraph::eLM_Default)
  {
    src += csGL4ShaderGraphLightData::Get().DiffuseLightingDefault;
    src += csGL4ShaderGraphLightData::Get().DiffuseLightingAmbient;
    src += csGL4ShaderGraphLightData::Get().DiffuseLightingDiffuse;
    src += csGL4ShaderGraphLightData::Get().DiffuseLightingSpecular;
  }
  else if(m_shaderGraph->GetLightingMode() == csShaderGraph::eLM_Attenuated)
  {
    src += csGL4ShaderGraphLightData::Get().DiffuseLightingAttenuated;
  }

  if (m_shaderGraph->GetRenderQueue() == eRenderQueue::Transparency)
  {
    src += csGL4ShaderGraphLightData::Get().DiffuseLightingShadowInline;
  }
  else
  {
    src += csGL4ShaderGraphLightData::Get().DiffuseLightingShadowMap;
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

  src += "  vec3 norm = normalize (cs_vs_out_WorldNormal);\n";

  if (m_shaderGraph->GetLightingMode() == csShaderGraph::eLM_Default)
  {
    if (tangentsNeeded || hasNormalInput)
    {
      src += R"(
  vec3 tang = normalize (cs_vs_out_WorldTangent);
  vec3 binormal = normalize (cross(norm, tang));
  tang = cross(binormal, norm);

  mat3 normalMatrix = mat3(tang, binormal, norm);
)";
    }
  }

  for (auto node: nodes)
  {
    if (std::find(alphaNodes.begin(), alphaNodes.end(), node) != alphaNodes.end())
    {
      // this node is already processed
      continue;
    }
    const std::string &decl = m_nodeVariables[node].StagedDecl("cs_vs_out_");
    if (!decl.empty())
    {
      src += "  " + decl + "\n";
    }
  }
  if (hasNormalInput && m_shaderGraph->GetLightingMode() == csShaderGraph::eLM_Default)
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
  auto alpha     = GetInputValue(m_shaderGraph->GetAlphaInput());
  if ((diffuse.Type & eSGValueType::Vector4) != eSGValueType::Invalid)
  {
    src += "  vec3 diffuse = " + diffuse.FullQualified() + ".xyz;\n";
  }
  else if ((diffuse.Type & eSGValueType::Vector3) != eSGValueType::Invalid)
  {
    src += "  vec3 diffuse = " + diffuse.FullQualified() + ";\n";
  }
  else
  {
    src += "  vec3 diffuse = vec3(" + diffuse.FullQualified() + ");\n";
  }
  src += "  float alpha = " + alpha.FullQualified() + ";\n";
  if (m_shaderGraph->GetLightingMode() == csShaderGraph::eLM_Default)
  {
    auto roughness = GetInputValue(m_shaderGraph->GetRoughnessInput());
    auto metallic  = GetInputValue(m_shaderGraph->GetMetallicInput());


    src += "  \n";
    src += "  float roughness = " + roughness.FullQualified() + ";\n";
    src += "  float metallic = " + metallic.FullQualified() + ";\n";

    src += R"(

  light_result_t light = calc_lights(cs_vs_out_WorldPosition, normal, cs_vs_out_CameraSpacePosition, cs_vs_out_CameraWorldPosition, 0.8, roughness);
  vec3 dielectric_light = light.diffuse  * diffuse + light.specular;
  vec3 metallic_light = light.specular * diffuse;

  cs_FragColor = vec4(mix(dielectric_light, metallic_light, metallic), alpha);
)";
  }
  else if (m_shaderGraph->GetLightingMode() == csShaderGraph::eLM_Attenuated)
  {
    src += R"(

  light_result_t light = calc_lights(cs_vs_out_WorldPosition, vec3(0, 0, 0), cs_vs_out_CameraSpacePosition, cs_vs_out_CameraWorldPosition, 0.0, 0.0);
  vec3 dielectric_light = light.diffuse  * diffuse;

  cs_FragColor = vec4(dielectric_light, alpha);
)";

  }
  else
  {
    src += "  cs_FragColor = vec4(diffuse, alpha);\n";
  }

  src += R"(
}

)";


  return src;

}


void csGL4ShaderGraphCompiler::GenerateForward(csGL4ShaderGraphCompiler::SourceBundle &bundle)
{
  bundle.vert = GenerateForward_Vert(bundle.attributes);
  if (!m_errorString.empty())
  { return; }
  bundle.geom = GenerateForward_Geom(bundle.attributes);
  if (!m_errorString.empty())
  { return; }
  bundle.frag = GenerateForward_Frag(bundle.attributes);
  if (!m_errorString.empty())
  { return; }
  bundle.attributes["ReceiveShadow"] = eMAT_Int;
}


}