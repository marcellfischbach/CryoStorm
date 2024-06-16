#include <ceCore/graphics/shadergraph/shadergraph.hh>


namespace ce
{

ShaderGraph::ShaderGraph()
  : SGNode("Shader Graph")
{
  m_diffuse = DefineInput("Diffuse", eSGValueType::Color3 | eSGValueType::Color4);
}

ShaderGraph::~ShaderGraph() noexcept
{

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


}