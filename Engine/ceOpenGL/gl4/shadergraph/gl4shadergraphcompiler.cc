
#include <ceOpenGL/gl4/shadergraph/gl4shadergraphcompiler.hh>
#include <ceOpenGL/gl4/shading/gl4program.hh>
#include <ceOpenGL/gl4/shading/gl4shader.hh>
#include <ceOpenGL/gl4/gl4exceptions.hh>
#include <algorithm>
#include <ceCore/graphics/shadergraph/sgnodes.hh>

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

  GenerateVariables();

  SourceBundle depth, deferred, forward, shadow, shadowPSSM, shadowPoint;
  GenerateDepth(depth);

  iShader* depthShader = Compile(depth);

  Material* material = new Material();
  material->SetShader(eRP_Depth, depthShader);

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


static void linearize(std::set<SGNode*> &untouched, SGNodeInput* input, std::vector<SGNode*> &linearized)
{
  SGNodeOutput *source = input->GetSource();
  if (!source)
  {
    return;
  }
  SGNode *sourceNode = source->GetNode();
  auto it = untouched.find(sourceNode);
  if (it == untouched.end())
  {
    return;
  }
  linearized.push_back(sourceNode);
  untouched.erase(it);

  for (size_t i=0, in=sourceNode->GetNumberOfInputs(); i<in; i++)
  {
    linearize (untouched, sourceNode->GetInput(i), linearized);
  }

}


void GL4ShaderGraphCompiler::LinearizeNodes()
{
  std::set<SGNode*> untouchedNodes;
  for (size_t i = 0, in = m_shaderGraph->GetNumberOfNodes(); i < in; i++)
  {
    untouchedNodes.insert(m_shaderGraph->GetNode(i));
  }

  m_linearizedNodes.clear();
  linearize(untouchedNodes, m_shaderGraph->GetDiffuseInput(), m_linearizedNodes);
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
          m_errorString = "Type mismatch. Output " + sourceNode->GetName() + "::" + source->GetName() + " types " + types_string(source->GetValueType()) +
            " do not match input " + node->GetName() + "::" + input->GetName() + " types " + types_string(input->GetTypes());
          return false;
        }
      }
    }
  }

  return true;
}

static GL4Shader* Compile(eGL4ShaderType type, const std::string& src)
{
  try {
    GL4Shader* shader = new GL4Shader(type);
    shader->SetSource(src);
    shader->Compile();
    return shader;
  }
  catch (GL4ShaderCompileException& exc)
  {
    printf("Compile error\n%s\n%s\n", src.c_str(), exc.what());
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


static bool Attach(GL4Program* program, eGL4ShaderType type, const std::string& src)
{
  if (src.empty())
  {
    return true;
  }

  printf("%s:\n%s\n", ShaderTypeName [type].c_str(), src.c_str());

  GL4Shader* shader = Compile(type, src);
  if (!shader)
  {
    return false;
  }

  program->AttachShader(shader);
  shader->Release();
  return true;
}


iShader* GL4ShaderGraphCompiler::Compile(SourceBundle& bundle)
{

  if (bundle.vert.empty() || bundle.frag.empty())
  {
    return nullptr;
  }
  GL4Program* program = new GL4Program();
  if (!Attach(program, eST_Vertex, bundle.vert)
    || !Attach(program, eST_TessEval, bundle.eval)
    || !Attach(program, eST_TessControl, bundle.ctrl)
    || !Attach(program, eST_Geometry, bundle.geom)
    || !Attach(program, eST_Fragment, bundle.frag))
  {
    program->Release();
    return nullptr;
  }

  try {
    program->Link();
    return program;
  }
  catch (GL4ProgramLinkException& exc)
  {
    printf("Unable to link program:\n%s\n", exc.what());
  }

  return nullptr;
}


void GL4ShaderGraphCompiler::ScanNeededVariables(std::set<SGNode*>& nodes, SGNodeInput* input)
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

std::vector<SGNode*> GL4ShaderGraphCompiler::ScanNeededVariables(std::vector<SGNodeInput*> inputs)
{
  std::set<SGNode*> needs;
  for (auto input : inputs)
  {
    ScanNeededVariables(needs, input);
  }

  std::vector<SGNode*> result;
  for (auto node : m_linearizedNodes)
  {
    if (needs.contains(node))
    {
      result.push_back(node);
    }
  }
  return result;
}


iShaderGraphCompiler* GL4ShaderGraphCompilerFactory::Create() const
{
  return new GL4ShaderGraphCompiler();
}

}
