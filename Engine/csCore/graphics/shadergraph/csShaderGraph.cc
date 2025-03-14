#include "csShaderGraph.hh"
#include <cmath>

namespace cs
{

static const size_t IDX_DIFFUSE = 0;
static const size_t IDX_ALPHA = 1;
static const size_t IDX_ROUGHNESS = 2;
static const size_t IDX_NORMAL = 3;
static const size_t IDX_METALLIC = 4;



csShaderGraph::ShaderGraphNode::ShaderGraphNode()
: csSGNode("Shader Graph")
{
  SetKey("Shader Graph");
  m_diffuse = DefineInput("Diffuse", eSGValueType::Float | eSGValueType::Vector3 | eSGValueType::Vector4,
                          csSGNodeInput::eM_Modifiable);
  m_alpha = DefineInput("Alpha", eSGValueType::Float, csSGNodeInput::eM_Modifiable);
  m_roughness = DefineInput("Roughness", eSGValueType::Float, csSGNodeInput::eM_Modifiable);
  m_normal = DefineInput("Normal", eSGValueType::Vector3, csSGNodeInput::eM_Modifiable);
  m_metallic = DefineInput("Metallic", eSGValueType::Float, csSGNodeInput::eM_Modifiable);


  m_roughness->SetScalar(0.5f);
  m_alpha->SetScalar(1.0f);
  m_metallic->SetScalar(0.0f);
}

void csShaderGraph::ShaderGraphNode::CalcIOTypes()
{

}

csSGNode *csShaderGraph::Root()
{
  return m_node;
}

csOwned<csSGNode> csShaderGraph::Add(const cs::csClass *nodeClass, const std::string &key)
{
  if (!nodeClass->IsInstanceOf<csSGNode>())
  {
    return nullptr;
  }

  csRef<iObject> obj = nodeClass->CreateInstance();
  if (!obj)
  {
    return nullptr;
  }

  auto node = obj->Query<csSGNode>();
  if (!node)
  {
    return nullptr;
  }
  node->SetKey(key);
  m_nodes.emplace_back(node);
  return node;

}

csOwned<csSGResourceNode> csShaderGraph::AddResource(const cs::csClass *nodeClass,
                                                     const std::string &key,
                                                     const std::string &resourceName)
{
  if (!nodeClass->IsInstanceOf<csSGResourceNode>())
  {
    return nullptr;
  }
  csRef<iObject> obj = nodeClass->CreateInstance();
  if (!obj)
  {
    return nullptr;
  }

  auto node = obj->Query<csSGResourceNode>();
  if (!node)
  {
    return nullptr;
  }
  node->SetKey(key);
  node->SetResourceName(resourceName);
  m_nodes.emplace_back(node);
  return node;
}

bool csShaderGraph::Remove(cs::csSGNode *node)
{
  auto it = std::find(m_nodes.begin(), m_nodes.end(), node);
  if (it == m_nodes.end())
  {
    return false;
  }


  for (int i = 0; i < node->GetNumberOfInputs(); ++i)
  {
    csSGNodeInput *pInput = node->GetInput(i);
    if (pInput && pInput->GetSource())
    {
      pInput->GetSource()->Remove(pInput);
      pInput->SetSource(nullptr);
    }
  }

  for (int i = 0; i < node->GetNumberOfOutputs(); ++i)
  {
    csSGNodeOutput *pOutput = node->GetOutput(i);
    if (pOutput)
    {
      for (const auto &outInput: std::vector<csRef<csSGNodeInput>>(pOutput->GetInputs()))
      {
        outInput->SetSource(nullptr);
        pOutput->Remove(outInput);
      }
    }
  }

  m_nodes.erase(it);
  return true;
}

void csShaderGraph::BindDiffuse(csSGNode *node, size_t outputIdx)
{
  m_node->Bind(IDX_DIFFUSE, node, outputIdx);
}

csSGNodeInput *csShaderGraph::GetDiffuseInput() const
{
  return m_node->m_diffuse;
}


void csShaderGraph::BindAlpha(csSGNode *node, size_t outputIdx)
{
  m_node->Bind(IDX_ALPHA, node, outputIdx);
}

csSGNodeInput *csShaderGraph::GetAlphaInput() const
{
  return m_node->m_alpha;
}

void csShaderGraph::BindRoughness(cs::csSGNode *node, size_t outputIdx)
{
  m_node->Bind(IDX_ROUGHNESS, node, outputIdx);
}

csSGNodeInput *csShaderGraph::GetRoughnessInput() const
{
  return m_node->m_roughness;
}

void csShaderGraph::BindNormal(cs::csSGNode *node, size_t outputIdx)
{
  m_node->Bind(IDX_NORMAL, node, outputIdx);
}

csSGNodeInput *csShaderGraph::GetNormalInput() const
{
  return m_node->m_normal;
}

void csShaderGraph::BindMetallic(cs::csSGNode *node, size_t outputIdx)
{
  m_node->Bind(IDX_METALLIC, node, outputIdx);
}

csSGNodeInput *csShaderGraph::GetMetallicInput() const
{
  return m_node->m_metallic;
}


size_t csShaderGraph::GetNumberOfNodes() const
{
  return m_nodes.size();
}

csSGNode *csShaderGraph::GetNode(size_t idx)
{
  if (idx >= m_nodes.size())
  {
    return nullptr;
  }
  return m_nodes[idx];
}

const csSGNode *csShaderGraph::GetNode(size_t idx) const
{
  if (idx >= m_nodes.size())
  {
    return nullptr;
  }
  return m_nodes[idx];
}

csSGNode *csShaderGraph::GetNode(const std::string &key)
{
  for (auto &node: m_nodes)
  {
    if (node->GetKey() == key)
    {
      return node;
    }
  }
  return nullptr;
}


const csSGNode *csShaderGraph::GetNode(const std::string &key) const
{
  for (const auto &node: m_nodes)
  {
    if (node->GetKey() == key)
    {
      return node;
    }
  }
  return nullptr;
}


void csShaderGraph::SetReceiveShadow(bool receiveShadow)
{
  m_receiveShadow = receiveShadow;
}

bool csShaderGraph::IsReceiveShadow() const
{
  return m_receiveShadow;
}

void csShaderGraph::SetAlphaDiscard(float threshold, eCompareFunc func)
{
  m_alphaDiscard_Threshold = threshold;
  m_alphaDiscard_Func = func;
}

float csShaderGraph::GetAlphaDiscard_Threshold() const
{
  return m_alphaDiscard_Threshold;
}

eCompareFunc csShaderGraph::GetAlphaDiscard_Func() const
{
  return m_alphaDiscard_Func;
}

/*
void csShaderGraph::SetDefault(const std::string &attribute, size_t count, float *floats)
{
  Default def{};
  def.name = attribute;
  count = std::min(static_cast<size_t>(16), count);
  memcpy(def.floats.data(), floats, sizeof(float) * count);

  m_defaults.push_back(def);
}

void csShaderGraph::SetDefault(const std::string &attribute, size_t count, int *ints)
{
  Default def{};
  def.name = attribute;
  count = std::min(static_cast<size_t>(4), count);
  memcpy(def.ints.data(), ints, sizeof(int) * count);

  m_defaults.push_back(def);
}

void csShaderGraph::SetDefault(const std::string &attribute, cs::iTexture *texture)
{
  Default def{};
  def.name = attribute;
  def.texture = texture;

  m_defaults.push_back(def);
}


const csShaderGraph::Default *csShaderGraph::GetDefault(const std::string &name) const
{
  for (auto &def: m_defaults)
  {
    if (def.name == name)
    {
      return &def;
    }
  }
  return nullptr;
}
*/
void csShaderGraph::SetLightingMode(eLightingMode lightingMode)
{
  m_lightingMode = lightingMode;
}

csShaderGraph::eLightingMode csShaderGraph::GetLightingMode() const
{
  return m_lightingMode;
}

void csShaderGraph::SetBlendingMode(eBlendingMode blendingMode)
{
  m_blendingMode = blendingMode;
}


csShaderGraph::eBlendingMode csShaderGraph::GetBlendingMode() const
{
  return m_blendingMode;
}


}

