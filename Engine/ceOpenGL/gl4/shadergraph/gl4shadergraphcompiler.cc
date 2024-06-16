
#include <ceOpenGL/gl4/shadergraph/gl4shadergraphcompiler.hh>
#include <algorithm>

namespace ce::opengl
{


Material* GL4ShaderGraphCompiler::Compile(ce::ShaderGraph* shaderGraph)
{
  m_shaderGraph = shaderGraph;
  m_errorString = "Uncompiled";

  if (CheckForCycle())
  {
    return nullptr;
  }

  LinearizeNodes();

  if (!Verify())
  {
    return nullptr;
  }
  printf("GL4ShaderGraphCompiler::Compile\n");
  m_errorString = "Unknown error";
  return nullptr;
}

const std::string& GL4ShaderGraphCompiler::GetError() const
{
  return m_errorString;
}


static bool has_cycle(const SGNode* node, const SGNode* referenceNode)
{
  for (size_t i = 0, in = node->GetNumberOfInputs(); i < in; i++)
  {
    const SGNodeInput* input = node->GetInput(i);
    const SGNodeOutput* source = input->GetSource();
    const SGNode* sourceNode = source ? source->GetNode() : nullptr;
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


static bool has_cycle(const SGNode* node)
{
  return has_cycle(node, node);
}

bool GL4ShaderGraphCompiler::CheckForCycle()
{
  for (size_t i = 0, in = m_shaderGraph->GetNumberOfNodes(); i < in; i++)
  {
    const SGNode* node = m_shaderGraph->GetNode(i);
    if (has_cycle(node))
    {
      m_errorString = "Cycle detected for node " + node->GetName();
      return true;
    }
  }
  return false;
}

/**
 * Checks wether reference is an ancestor of node.
 */
bool is_ancestor(SGNode* reference, SGNode* node)
{
  if (node == reference)
  {
    return true;
  }

  for (size_t i = 0, in = node->GetNumberOfInputs(); i < in; i++)
  {
    SGNodeInput* input = node->GetInput(i);
    SGNodeOutput* source = input->GetSource();
    SGNode* sourceNode = source ? source->GetNode() : nullptr;
    if (sourceNode)
    {
      if (is_ancestor(reference, sourceNode))
      {
        return true;
      }
    }
  }
  return false;
}



void GL4ShaderGraphCompiler::LinearizeNodes()
{
  for (size_t i = 0, in = m_shaderGraph->GetNumberOfNodes(); i < in; i++)
  {
    m_linearizedNodes.push_back(m_shaderGraph->GetNode(i));
  }
  std::sort(m_linearizedNodes.begin(), m_linearizedNodes.end(), is_ancestor);

}


static std::string types_string(eSGValueType types)
{
#define FIRST if (!first) { res += ", "; } first = false;
#define TEST(x) if ((types & eSGValueType::x) == eSGValueType::x) { FIRST res += #x; }
  bool first = true;

  std::string res = "[";
  TEST(Scalar)
    TEST(Vector2)
    TEST(Vector3)
    TEST(Vector4)
    TEST(Color3)
    TEST(Color4)
#undef FIRST
#undef TEST
#undef ELSE_TEST
    res += "]";
  return res;
}

bool GL4ShaderGraphCompiler::Verify()
{
  // we can calc the io types and check the output to input type in one step 
  // because all output values, that are needed for the inputs,  are check already
  // because the nodes are linearized already
  for (auto node : m_linearizedNodes)
  {
    if (!Verify(node))
    {
      return false;
    }
  }
  return Verify(m_shaderGraph);
}

bool GL4ShaderGraphCompiler::Verify(SGNode* node)
{
  node->CalcIOTypes();


  for (size_t i = 0, in = node->GetNumberOfInputs(); i < in; i++)
  {
    SGNodeInput* input = node->GetInput(i);
    SGNodeOutput* source = input->GetSource();

    if (source)
    {
      if ((source->GetValueType() & input->GetTypes()) == eSGValueType::Invalid)
      {
        SGNode* sourceNode = source->GetNode();
        if (sourceNode)
        {
          m_errorString = "Type mismatch. Output " + sourceNode->GetName() + "::" + source->GetName() + " types " + types_string(source->GetTypes()) +
            " do not match input " + node->GetName() + "::" + input->GetName() + " types " + types_string(input->GetTypes());
          return false;
        }
      }
    }
  }

  return true;
}





iShaderGraphCompiler* GL4ShaderGraphCompilerFactory::Create() const
{
  return new GL4ShaderGraphCompiler();
}

}
