#include <ceCore/graphics/shadergraph/shadergraph.hh>


namespace ce
{

ShaderGraph::ShaderGraph()
  : SGNode("Shader Graph")
{
  m_diffuse = DefineInput("Diffuse", eSGValueType::Float | eSGValueType::Vector3 | eSGValueType::Vector4);
}

ShaderGraph::~ShaderGraph() noexcept
{

}

void ShaderGraph::BindDiffuse(SGNode* node, size_t outputIdx)
{
  Bind(0, node, outputIdx);
}

SGNodeInput* ShaderGraph::GetDiffuseInput()
{
  return m_diffuse;
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