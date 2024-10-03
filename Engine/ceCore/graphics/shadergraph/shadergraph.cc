#include <ceCore/graphics/shadergraph/shadergraph.hh>
#include <cmath>

namespace ce
{

static const size_t IDX_DIFFUSE = 0;
static const size_t IDX_ALPHA = 1;
static const size_t IDX_ROUGHNESS = 2;
static const size_t IDX_NORMAL = 3;
static const size_t IDX_METALLIC = 4;

ShaderGraph::ShaderGraph()
    : SGNode("Shader Graph")

{
  SetKey("Shader Graph");
  m_diffuse = DefineInput("Diffuse", eSGValueType::Float | eSGValueType::Vector3 | eSGValueType::Vector4);
  m_alpha = DefineInput("Alpha", eSGValueType::Float);
  m_roughness = DefineInput("Roughness", eSGValueType::Float);
  m_normal = DefineInput("Normal", eSGValueType::Vector3);
  m_metallic = DefineInput("Metallic", eSGValueType::Float);


  m_roughness->SetScalar(0.5f);
  m_alpha->SetScalar(1.0f);
  m_metallic->SetScalar(0.0f);
}

ShaderGraph::~ShaderGraph() noexcept
{

}

SGNode *ShaderGraph::Add(const ce::Class *nodeClass, const std::string &key)
{
  if (!nodeClass->IsInstanceOf<SGNode>())
  {
    return nullptr;
  }

  iObject *obj = nodeClass->CreateInstance();
  if (!obj)
  {
    return nullptr;
  }

  auto node = obj->Query<SGNode>();
  if (!node)
  {
    obj->Release();
    return nullptr;
  }
  node->SetKey(key);
  m_nodes.push_back(node);
  return node;

}

SGResourceNode *ShaderGraph::AddResource(const ce::Class *nodeClass,
                                         const std::string &key,
                                         const std::string &resourceName)
{
  if (!nodeClass->IsInstanceOf<SGResourceNode>())
  {
    return nullptr;
  }
  iObject *obj = nodeClass->CreateInstance();
  if (!obj)
  {
    return nullptr;
  }

  auto node = obj->Query<SGResourceNode>();
  if (!node)
  {
    obj->Release();
    return nullptr;
  }
  node->SetKey(key);
  node->SetResourceName(resourceName);
  m_nodes.push_back(node);
  return node;
}

void ShaderGraph::BindDiffuse(SGNode *node, size_t outputIdx)
{
  Bind(IDX_DIFFUSE, node, outputIdx);
}

SGNodeInput *ShaderGraph::GetDiffuseInput()
{
  return m_diffuse;
}


void ShaderGraph::BindAlpha(SGNode *node, size_t outputIdx)
{
  Bind(IDX_ALPHA, node, outputIdx);
}

SGNodeInput *ShaderGraph::GetAlphaInput()
{
  return m_alpha;
}

void ShaderGraph::BindRoughness(ce::SGNode *node, size_t outputIdx)
{
  Bind(IDX_ROUGHNESS, node, outputIdx);
}

SGNodeInput *ShaderGraph::GetRoughnessInput()
{
  return m_roughness;
}

void ShaderGraph::BindNormal(ce::SGNode *node, size_t outputIdx)
{
  Bind(IDX_NORMAL, node, outputIdx);
}

SGNodeInput *ShaderGraph::GetNormalInput()
{
  return m_normal;
}

void ShaderGraph::BindMetallic(ce::SGNode *node, size_t outputIdx)
{
  Bind(IDX_METALLIC, node, outputIdx);
}

SGNodeInput *ShaderGraph::GetMetallicInput()
{
  return m_metallic;
}

void ShaderGraph::CalcIOTypes()
{

}

size_t ShaderGraph::GetNumberOfNodes() const
{
  return m_nodes.size();
}

SGNode *ShaderGraph::GetNode(size_t idx)
{
  if (idx >= m_nodes.size())
  {
    return nullptr;
  }
  return m_nodes[idx];
}

const SGNode *ShaderGraph::GetNode(size_t idx) const
{
  if (idx >= m_nodes.size())
  {
    return nullptr;
  }
  return m_nodes[idx];
}

SGNode *ShaderGraph::GetNode(const std::string &key)
{
  for (auto node: m_nodes)
  {
    if (node->GetKey() == key)
    {
      return node;
    }
  }
  return nullptr;
}


const SGNode *ShaderGraph::GetNode(const std::string &key) const
{
  for (const auto node: m_nodes)
  {
    if (node->GetKey() == key)
    {
      return node;
    }
  }
  return nullptr;
}


void ShaderGraph::SetReceiveShadow(bool receiveShadow)
{
  m_receiveShadow = receiveShadow;
}

bool ShaderGraph::IsReceiveShadow() const
{
  return m_receiveShadow;
}

void ShaderGraph::SetAlphaDiscard(float threshold, eCompareFunc func)
{
  m_alphaDiscard_Threshold = threshold;
  m_alphaDiscard_Func = func;
}

float ShaderGraph::GetAlphaDiscard_Threshold() const
{
  return m_alphaDiscard_Threshold;
}

eCompareFunc ShaderGraph::GetAlphaDiscard_Func() const
{
  return m_alphaDiscard_Func;
}


void ShaderGraph::SetDefault(const std::string &attribute, size_t count, float *floats)
{
  Default def{};
  def.name = attribute;
  count = std::min(static_cast<size_t>(16), count);
  memcpy(def.floats.data(), floats, sizeof(float) * count);

  m_defaults.push_back(def);
}

void ShaderGraph::SetDefault(const std::string &attribute, size_t count, int *ints)
{
  Default def{};
  def.name = attribute;
  count = std::min(static_cast<size_t>(4), count);
  memcpy(def.ints.data(), ints, sizeof(int) * count);

  m_defaults.push_back(def);
}

void ShaderGraph::SetDefault(const std::string &attribute, ce::iTexture *texture)
{
  Default def{};
  def.name = attribute;
  CS_SET(def.texture, texture);

  m_defaults.push_back(def);
}


const ShaderGraph::Default *ShaderGraph::GetDefault(const std::string &name) const
{
  for (auto &def : m_defaults)
  {
    if (def.name == name)
    {
      return &def;
    }
  }
  return nullptr;
}

void ShaderGraph::SetLightingMode(eLightingMode lightingMode)
{
  m_lightingMode = lightingMode;
}

ShaderGraph::eLightingMode ShaderGraph::GetLightingMode() const
{
  return m_lightingMode;
}

void ShaderGraph::SetBlendingMode(eBlendingMode blendingMode)
{
  m_blendingMode = blendingMode;
}


ShaderGraph::eBlendingMode ShaderGraph::GetBlendingMode() const
{
  return m_blendingMode;
}


void ShaderGraph::SetQueue(eRenderQueue queue)
{
  m_queue = queue;
}

eRenderQueue ShaderGraph::GetQueue() const
{
  return m_queue;
}
}

