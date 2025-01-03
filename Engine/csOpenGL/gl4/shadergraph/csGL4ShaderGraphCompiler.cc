
#include <csOpenGL/gl4/shadergraph/csGL4ShaderGraphCompiler.hh>
#include <csOpenGL/gl4/shading/csGL4Program.hh>
#include <csOpenGL/gl4/shading/csGL4Shader.hh>
#include <csOpenGL/gl4/csGL4Exceptions.hh>
#include <algorithm>
#include <csCore/graphics/shadergraph/csSGNodes.hh>
#include <csCore/resource/csAssetManager.hh>
#include <csCore/resource/csTextFile.hh>
#include <csCore/csSettings.hh>

namespace cs::opengl
{


csMaterial *csGL4ShaderGraphCompiler::Compile(cs::csShaderGraph *shaderGraph, const Parameters &parameters)
{
  m_shaderGraph = shaderGraph;
  m_errorString = "";
  m_parameters = parameters;

  if (!csGL4ShaderGraphLightData::Get().Valid)
  {
    return nullptr;
  }
  if (CheckForCycle())
  {
    return nullptr;
  }
  if (!VerifyResources())
  {
    return nullptr;
  }

  // TODO: time for code optimization

  LinearizeNodes();

  if (!VerifyNodesType())
  {
    return nullptr;
  }

  GenerateVariables();

  SourceBundle depth, forward, shadow, shadowPSSM, shadowPoint;
  GenerateDepth(depth);
  GenerateForward(forward);

  iShader *depthShader = Compile(depth, "Depth");
  iShader *forwardShader = Compile(forward, "Forward");

  csMaterial *material = new csMaterial();
  material->SetShader(eRP_Depth, depthShader);
  material->SetShader(eRP_Forward, forwardShader);

  if (m_shaderGraph->GetLightingMode() == csShaderGraph::eLM_Default
      && m_shaderGraph->GetBlendingMode() == csShaderGraph::eBM_Off)
  {
    SourceBundle gbuffer;
    GenerateGBuffer(gbuffer);
    iShader *gbufferShader = Compile(gbuffer, "GBuffer");
    material->SetShader(eRP_GBuffer, gbufferShader);
  }

  material->SetRenderQueue(shaderGraph->GetQueue());


  switch (m_shaderGraph->GetLightingMode())
  {
    case csShaderGraph::eLM_Default:
      material->SetShadingMode(eShadingMode::Shaded);
      material->SetBlending(false);
      break;

    case csShaderGraph::eLM_Attenuated:
      material->SetShadingMode(eShadingMode::Shaded);
      material->SetBlending(true);
      break;

    case csShaderGraph::eLM_Unlit:
      material->SetShadingMode(eShadingMode::Unshaded);
      material->SetBlending(true);
      break;

  }
  switch (m_shaderGraph->GetBlendingMode())
  {
    case csShaderGraph::eBM_Off:
      material->SetBlending(false);
      material->SetBlendFactor(eBlendFactor::One, eBlendFactor::Zero);
      material->SetDepthWrite(true);
      break;

    case csShaderGraph::eBM_Alpha:
      material->SetBlending(true);
      material->SetBlendFactor(eBlendFactor::SrcAlpha, eBlendFactor::OneMinusSrcAlpha);
      material->SetDepthWrite(false);
      break;

    case csShaderGraph::eBM_Add:
      material->SetBlending(true);
      material->SetBlendFactor(eBlendFactor::SrcAlpha, eBlendFactor::One);
      material->SetDepthWrite(false);
      break;
  }
  if (!m_errorString.empty())
  {
    CS_RELEASE(material);
    return nullptr;
  }

  for (const auto &attrib: depth.attributes)
  {
    if (material->IndexOf(attrib.first) == csMaterial::UndefinedIndex)
    {
      material->RegisterAttribute(attrib.first, attrib.second);
    }
  }
  for (const auto &attrib: forward.attributes)
  {
    if (material->IndexOf(attrib.first) == csMaterial::UndefinedIndex)
    {
      material->RegisterAttribute(attrib.first, attrib.second);
    }
  }

  size_t receiveShadowIndex = material->IndexOf("ReceiveShadow");
  if (receiveShadowIndex != csMaterial::UndefinedIndex)
  {
    material->SetInt(receiveShadowIndex, m_shaderGraph->IsReceiveShadow() ? 1 : 0);
  }


  SetMaterialDefaults(material);
  m_errorString = "Unknown error";
  return material;
}

const std::string &csGL4ShaderGraphCompiler::GetError() const
{
  return m_errorString;
}


static bool has_cycle(const csSGNode *node, const csSGNode *referenceNode)
{
  for (size_t i = 0, in = node->GetNumberOfInputs(); i < in; i++)
  {
    const csSGNodeInput *input = node->GetInput(i);
    const csSGNodeOutput *source = input->GetSource();
    const csSGNode *sourceNode = source ? source->GetNode() : nullptr;
    if (sourceNode)
    {
      if (sourceNode == referenceNode)
      {
        return true;
      }
      if (has_cycle(sourceNode, referenceNode))
      {
        return true;
      }
    }
  }
  return false;

}


static bool has_cycle(const csSGNode *node)
{
  return has_cycle(node, node);
}

bool csGL4ShaderGraphCompiler::CheckForCycle()
{
  for (size_t i = 0, in = m_shaderGraph->GetNumberOfNodes(); i < in; i++)
  {
    const csSGNode *node = m_shaderGraph->GetNode(i);
    if (has_cycle(node))
    {
      m_errorString = "Cycle detected for node " + node->GetName();
      return true;
    }
  }
  return false;
}


static void linearize(std::set<csSGNode *> &untouched, csSGNodeInput *input, std::vector<csSGNode *> &linearized)
{
  csSGNodeOutput *source = input->GetSource();
  if (!source)
  {
    return;
  }
  csSGNode *sourceNode = source->GetNode();
  auto it = untouched.find(sourceNode);
  if (it == untouched.end())
  {
    return;
  }
  linearized.push_back(sourceNode);
  untouched.erase(it);

  for (size_t i = 0, in = sourceNode->GetNumberOfInputs(); i < in; i++)
  {
    linearize(untouched, sourceNode->GetInput(i), linearized);
  }

}


void csGL4ShaderGraphCompiler::LinearizeNodes()
{
  std::set<csSGNode *> untouchedNodes;
  for (size_t i = 0, in = m_shaderGraph->GetNumberOfNodes(); i < in; i++)
  {
    untouchedNodes.insert(m_shaderGraph->GetNode(i));
  }

  m_linearizedNodes.clear();
  for (size_t i = 0, in = m_shaderGraph->GetNumberOfInputs(); i < in; i++)
  {
    linearize(untouchedNodes, m_shaderGraph->GetInput(i), m_linearizedNodes);
  }
  std::reverse(m_linearizedNodes.begin(), m_linearizedNodes.end());
}


static std::string types_string(eSGValueType types)
{
#define FIRST if (!first) { res += ", "; } first = false;
#define TEST(x) if ((types & eSGValueType::x) == eSGValueType::x) { FIRST res += #x; }
  bool first = true;

  std::string res = "[";
  TEST(Float)
  TEST(Vector2)
  TEST(Vector3)
  TEST(Vector4)
#undef FIRST
#undef TEST
#undef ELSE_TEST
  res += "]";
  return res;
}

bool csGL4ShaderGraphCompiler::VerifyNodesType()
{
  // we can calc the io types and check the output to input type in one step
  // because all output values, that are needed for the inputs,  are check already
  // because the nodes are linearized already
  for (auto node: m_linearizedNodes)
  {
    if (!VerifyNodeType(node))
    {
      return false;
    }
  }
  return VerifyNodeType(m_shaderGraph);
}

bool csGL4ShaderGraphCompiler::VerifyNodeType(csSGNode *node)
{
  node->CalcIOTypes();


  for (size_t i = 0, in = node->GetNumberOfInputs(); i < in; i++)
  {
    csSGNodeInput *input = node->GetInput(i);
    csSGNodeOutput *source = input->GetSource();

    if (source)
    {
      if ((source->GetValueType() & input->GetTypes()) == eSGValueType::Invalid)
      {
        csSGNode *sourceNode = source->GetNode();
        if (sourceNode)
        {
          m_errorString = "Type mismatch. Output " + sourceNode->GetName() + "::" + source->GetName() + " types " +
                          types_string(source->GetValueType()) +
                          " do not match input " + node->GetName() + "::" + input->GetName() + " types " +
                          types_string(input->GetTypes());
          return false;
        }
      }
    }
    else
    {
      if (node->IsInstanceOf<csSGTexture1D>() || node->IsInstanceOf<csSGTexture3D>())
      {
        m_errorString = "Invalid input. No default texture coordinates for " + node->GetName() +
                        " existing. Assign texture coordinates";
        return false;
      }
    }

  }

  return true;
}

bool csGL4ShaderGraphCompiler::VerifyResources()
{
  std::map<std::string, csSGResourceNode *> resources;
  for (size_t i = 0, in = m_shaderGraph->GetNumberOfNodes(); i < in; ++i)
  {
    csSGNode *node = m_shaderGraph->GetNode(i);
    csSGResourceNode *resourceNode = node->Query<csSGResourceNode>();
    if (resourceNode)
    {
      auto it = resources.find(resourceNode->GetResourceName());
      if (it != resources.end())
      {
        if (it->second->GetClass() != resourceNode->GetClass())
        {
          m_errorString = "Resource [" + resourceNode->GetResourceName() + "] " + resourceNode->GetClass()->GetName()
                          + " previously declared as " + it->second->GetClass()->GetName();
          return false;
        }
      }
      resources[resourceNode->GetResourceName()] = resourceNode;
    }
  }
  return true;
}

std::string line_number(size_t number)
{
  if (number >= 100)
  {
    return std::to_string(number);
  }
  if (number >= 10)
  {
    return "0" + std::to_string(number);
  }

  return "00" + std::to_string(number);
}

std::string replace_all(const std::string &input, const std::string &from, const std::string &to)
{
  std::string str = input;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos)
  {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
  }
  return str;
}

std::string annotate_with_line_numbers(const std::string &code)
{
  std::string result;
  std::string str = replace_all(code, "\r\n", "\n");
  str = replace_all(str, "\r", "\n");

  size_t line_no = 1;
  bool newLine = true;
  bool hadR = false;
  for (const auto &ch: str)
  {
    if (newLine)
    {
      result += "(" + line_number(line_no) + ") ";
      newLine = false;
    }
    if (ch == '\n')
    {
      newLine = true;
      line_no++;
    }

    result += ch;
  }
  return result;
}

static csGL4Shader *Compile(eGL4ShaderType type, const std::string &src)
{
  try
  {
    csGL4Shader *shader = new csGL4Shader(type);
    shader->SetSource(src);
    shader->Compile();
    return shader;
  }
  catch (csGL4ShaderCompileException &exc)
  {
    printf("Compile error\n%s\n%s\n", annotate_with_line_numbers(src).c_str(), exc.what());
    fflush(stdout);
  }
  return nullptr;
}

static std::string ShaderTypeName[] = {
    "Vertex",
    "TessEval",
    "TessControl",
    "Geometry",
    "Fragment",
    "Compute"
};


static bool Attach(csGL4Program *program,
                   eGL4ShaderType type,
                   const std::string &src,
                   bool debugSources,
                   const std::string &pathName,
                   const std::string &debugName)
{
  if (src.empty())
  {
    return true;
  }

  if (debugSources)
  {
    printf("%s.%s [%s]:\n%s\n\n\n", pathName.c_str(), ShaderTypeName[type].c_str(), debugName.c_str(), src.c_str());
    fflush(stdout);
  }

  csGL4Shader *shader = Compile(type, src);
  if (!shader)
  {
    return false;
  }

  program->AttachShader(shader);
  shader->Release();
  return true;
}


iShader *csGL4ShaderGraphCompiler::Compile(SourceBundle &bundle, const std::string &pathName)
{

  if (bundle.vert.empty() || bundle.frag.empty())
  {
    return nullptr;
  }
  csGL4Program *program = new csGL4Program();
  if (!Attach(program, eST_Vertex, bundle.vert, m_parameters.DebugSources, pathName, m_parameters.DebugName)
      || !Attach(program, eST_TessEval, bundle.eval, m_parameters.DebugSources, pathName, m_parameters.DebugName)
      || !Attach(program, eST_TessControl, bundle.ctrl, m_parameters.DebugSources, pathName, m_parameters.DebugName)
      || !Attach(program, eST_Geometry, bundle.geom, m_parameters.DebugSources, pathName, m_parameters.DebugName)
      || !Attach(program, eST_Fragment, bundle.frag, m_parameters.DebugSources, pathName, m_parameters.DebugName))
  {
    program->Release();
    return nullptr;
  }

  try
  {
    program->Link();

    for (const auto &attribute: bundle.attributes)
    {
      program->RegisterAttribute(attribute.first);
    }
    return program;
  }
  catch (csGL4ProgramLinkException &exc)
  {
    printf("Unable to link program:\n%s\n", exc.what());
  }


  return nullptr;
}


void csGL4ShaderGraphCompiler::ScanNeededVariables(std::set<csSGNode *> &nodes, csSGNodeInput *input)
{
  auto source = input->GetSource();
  if (!source)
  {
    return;
  }

  auto sourceNode = source->GetNode();
  nodes.insert(sourceNode);


  for (size_t i = 0, in = sourceNode->GetNumberOfInputs(); i < in; i++)
  {
    ScanNeededVariables(nodes, sourceNode->GetInput(i));
  }
}

std::vector<csSGNode *> csGL4ShaderGraphCompiler::ScanNeededVariables(std::vector<csSGNodeInput *> inputs)
{
  std::vector<csSGNode *> result;
  if (inputs.empty())
  {
    return result;
  }
  std::set<csSGNode *> needs;
  for (auto input: inputs)
  {
    ScanNeededVariables(needs, input);
  }

  for (auto node: m_linearizedNodes)
  {
    if (needs.contains(node))
    {
      result.push_back(node);
    }
  }
  return result;
}

const csGL4ShaderGraphLightData &csGL4ShaderGraphLightData::Get()
{
  static csGL4ShaderGraphLightData data;
  return data;
}

csGL4ShaderGraphLightData::csGL4ShaderGraphLightData()
{
  Valid = false;
  const csSettingsFile &gfxSettings = csSettings::Get().Graphics();

  csTextFile *txt = csAssetManager::Get()->Get<csTextFile>(
      "/shaders/gl4/shadergraph/diffuse/diffuse_default_lighting.glsl");
  if (!txt)
  {
    return;
  }
  DiffuseLightingDefault = txt->GetContent();
  txt->Release();

  txt = csAssetManager::Get()->Get<csTextFile>("/shaders/gl4/shadergraph/diffuse/diffuse_attenuated_lighting.glsl");
  if (!txt)
  {
    return;
  }
  DiffuseLightingAttenuated = txt->GetContent();
  txt->Release();


  std::string ambientFile = gfxSettings.GetText("ambient", "null");
  txt = csAssetManager::Get()->Get<csTextFile>("/shaders/gl4/shadergraph/lighting/ambient_" + ambientFile + ".glsl");
  if (!txt)
  {
    return;
  }
  DiffuseLightingAmbient = txt->GetContent();


  std::string diffuseFile = gfxSettings.GetText("diffuse", "null");
  txt = csAssetManager::Get()->Get<csTextFile>("/shaders/gl4/shadergraph/lighting/diffuse_" + diffuseFile + ".glsl");
  if (!txt)
  {
    return;
  }
  DiffuseLightingDiffuse = txt->GetContent();

  std::string specularFile = gfxSettings.GetText("specular", "null");
  txt = csAssetManager::Get()->Get<csTextFile>("/shaders/gl4/shadergraph/lighting/specular_" + specularFile + ".glsl");
  if (!txt)
  {
    return;
  }
  DiffuseLightingSpecular = txt->GetContent();

  txt = csAssetManager::Get()->Get<csTextFile>("/shaders/gl4/shadergraph/lighting/shadow_map.glsl");
  if (!txt)
  {
    return;
  }
  DiffuseLightingShadowMap = txt->GetContent();

  txt = csAssetManager::Get()->Get<csTextFile>("/shaders/gl4/shadergraph/lighting/shadow_inline.glsl");
  if (!txt)
  {
    return;
  }
  DiffuseLightingShadowInline = txt->GetContent();

  Valid = true;
}

bool csGL4ShaderGraphCompiler::CollectAttributes(std::vector<csSGNode *> &nodes,
                                                 std::map<std::string, eMaterialAttributeType> &attributes)
{
  for (const auto &node: nodes)
  {
    auto resourceNode = node->Query<csSGResourceNode>();
    if (resourceNode)
    {
      auto it = attributes.find(resourceNode->GetResourceName());
      if (it != attributes.end() && it->second != resourceNode->GetMatType())
      {
        m_errorString = "The Attribute [" + resourceNode->GetResourceName() + "] is given with multiples types";
        return false;
      }
      attributes[resourceNode->GetResourceName()] = resourceNode->GetMatType();
    }
  }
  return true;
}

bool csGL4ShaderGraphCompiler::IsNeedingTangent(const std::vector<csSGNode *> &nodes) const
{
  return false;
}


void csGL4ShaderGraphCompiler::SetMaterialDefaults(cs::csMaterial *material)
{
  std::vector<csSGNode *> nodes;
  for (int i = 0; i < m_shaderGraph->GetNumberOfNodes(); ++i)
  {
    csSGNode* node = m_shaderGraph->GetNode(i);
    if (!node || !node->IsInstanceOf<csSGResourceNode>())
    {
      continue;
    }
    csSGResourceNode* resource = node->Query<csSGResourceNode>();

    size_t idx = material->IndexOf(resource->GetResourceName());
    if (idx == csMaterial::UndefinedIndex)
    {
      continue;
    }

    const std::array<float, 16>& floats = resource->GetDefaultFloats();
    const std::array<int, 4>& ints = resource->GetDefaultInts();

    switch (resource->GetMatType())
    {
      case eMAT_Float:
        material->SetFloat(idx, floats[0]);
        break;
      case eMAT_Vec2:
        material->SetVector2f(idx, csVector2f(floats[0], floats[1]));
        break;
      case eMAT_Vec3:
        material->SetVector3f(idx, csVector3f(floats[0], floats[1], floats[2]));
        break;
      case eMAT_Vec4:
        material->SetVector4f(idx, csVector4f(floats[0], floats[1], floats[2], floats[3]));
        break;

      case eMAT_Matrix3:
        material->SetMatrix3f(idx, csMatrix3f(floats.data()));
        break;
      case eMAT_Matrix4:
        material->SetMatrix4f(idx, csMatrix4f(floats.data()));
        break;

      case eMAT_Int:
        material->SetInt(idx, ints[0]);
        break;

      case eMAT_Texture:
      {
        iTexture* texture = csAssetManager::Get()->Get<iTexture>(resource->GetDefaultLocator());
        material->SetTexture(idx, texture);
      }
        break;

      default:
        break;
    }
  }
}

iShaderGraphCompiler *csGL4ShaderGraphCompilerFactory::Create() const
{
  return new csGL4ShaderGraphCompiler();
}

}
