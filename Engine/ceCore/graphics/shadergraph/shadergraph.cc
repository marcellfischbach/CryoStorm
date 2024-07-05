#include <ceCore/graphics/shadergraph/shadergraph.hh>


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

void ShaderGraph::BindDiffuse(SGNode* node, size_t outputIdx)
{
  Bind(IDX_DIFFUSE, node, outputIdx);
}

SGNodeInput* ShaderGraph::GetDiffuseInput()
{
  return m_diffuse;
}


void ShaderGraph::BindAlpha(SGNode* node, size_t outputIdx)
{
  Bind(IDX_ALPHA, node, outputIdx);
}

SGNodeInput* ShaderGraph::GetAlphaInput()
{
  return m_alpha;
}

void ShaderGraph::BindRoughness(ce::SGNode *node, size_t outputIdx)
{
  Bind(IDX_ROUGHNESS, node, outputIdx);
}

SGNodeInput* ShaderGraph::GetRoughnessInput()
{
  return m_roughness;
}

void ShaderGraph::BindNormal(ce::SGNode *node, size_t outputIdx)
{
  Bind(IDX_NORMAL, node, outputIdx);
}

SGNodeInput* ShaderGraph::GetNormalInput()
{
  return m_normal;
}

void ShaderGraph::BindMetallic(ce::SGNode *node, size_t outputIdx)
{
  Bind(IDX_METALLIC, node, outputIdx);
}

SGNodeInput* ShaderGraph::GetMetallicInput()
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

SGNode* ShaderGraph::GetNode(size_t idx)
{
  if (idx >= m_nodes.size())
  {
    return nullptr;
  }
  return m_nodes[idx];
}

const SGNode* ShaderGraph::GetNode(size_t idx) const
{
  if (idx >= m_nodes.size())
  {
    return nullptr;
  }
  return m_nodes[idx];
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


}