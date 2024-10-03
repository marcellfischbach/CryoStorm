
#include <ceOpenGL/gl4/shadergraph/gl4shadergraphcompiler.hh>
#include <ceCore/graphics/shadergraph/sgnodes.hh>
#include <algorithm>

namespace cryo::opengl
{

void GL4ShaderGraphCompiler::GenerateVariables()
{
  for (auto node: m_linearizedNodes)
  {
    GenerateVariable(node);
    for (size_t i = 0, in = node->GetNumberOfOutputs(); i < in; i++)
    {
      auto output = node->GetOutput(i);
      GenerateVariable(output);
    }
  }
}


std::string get_gl_type(eSGValueType type)
{
  switch (type)
  {
    case eSGValueType::Invalid:
      return "int";
    case eSGValueType::Float:
      return "float";
    case eSGValueType::Vector2:
      return "vec2";
    case eSGValueType::Vector3:
      return "vec3";
    case eSGValueType::Vector4:
      return "vec4";
  }
  return "";
}

std::string get_operator(SGBinaryOperator::eOperator op)
{
  switch (op)
  {
    case SGBinaryOperator::Add:
      return "+";
    case SGBinaryOperator::Sub:
      return "-";
    case SGBinaryOperator::Mul:
      return "*";
    case SGBinaryOperator::Div:
      return "/";
    case SGBinaryOperator::Mod:
      return "%";
  }
  return "";
}


std::string stream_name(eVertexStream stream)
{
  switch (stream)
  {
    case eVS_Vertices:
      return "Vertices";
    case eVS_Normals:
      return "Normals";
    case eVS_Tangents:
      return "Tangents";
    case eVS_UV0:
      return "UV0";
    case eVS_UV1:
      return "UV1";
    case eVS_UV2:
      return "UV2";
    case eVS_UV3:
      return "UV3";
    case eVS_Colors:
      return "Colors";
    case eVS_BoneIndices:
      return "BoneIndices";
    case eVS_BoneWeights:
      return "BoneWeights";
  }
  return "";
}

void GL4ShaderGraphCompiler::AddStream(std::vector<StreamInput> &streams,
                                       cryo::eVertexStream stream,
                                       cryo::eSGValueType type)
{
  for (auto it = streams.begin(); it != streams.end(); it++)
  {
    if (it->Stream == stream)
    {
      return;
    }
  }
  streams.push_back({stream, type});

}

std::vector<GL4ShaderGraphCompiler::StreamInput> GL4ShaderGraphCompiler::FindStreams(std::vector<SGNode *> &nodes)
{

  std::vector<StreamInput> streams;
  for (auto                node: nodes)
  {
    auto streamNode = node->Query<SGStreamNode>();
    if (streamNode)
    {
      AddStream(streams, streamNode->GetStream(), streamNode->GetOutput()->GetValueType());
    }
    auto texture2DNode = node->Query<SGTexture2D>();
    if (texture2DNode)
    {
      if (!texture2DNode->GetInput(0)->GetSource())
      {
        AddStream(streams, eVS_UV, eSGValueType::Vector2);
      }
    }
  }

  std::sort(streams.begin(), streams.end(), [](StreamInput &a, StreamInput &b) -> bool {
    return a.Stream < b.Stream;
  });
  return streams;
}

void GL4ShaderGraphCompiler::AddResource(std::vector<ResourceInput> &resources,
                                         const std::string &resourceName,
                                         const std::string &resourceType,
                                         cryo::eMaterialAttributeType matType)
{
  for (auto it = resources.begin(); it != resources.end(); it++)
  {
    if (it->Name == resourceName)
    {
      return;
    }
  }

  resources.push_back({resourceName, resourceType, matType});
}

std::vector<GL4ShaderGraphCompiler::ResourceInput> GL4ShaderGraphCompiler::FindResources(std::vector<SGNode *> &nodes)
{
  std::vector<ResourceInput> resources;
  for (auto                  node: nodes)
  {
    auto resourceNode = node->Query<SGResourceNode>();
    if (resourceNode)
    {
      if (resourceNode->IsInstanceOf<SGTexture2D>())
      {
        AddResource(resources, resourceNode->GetResourceName(), "sampler2D", resourceNode->GetMatType());
      }
      else
      {
        AddResource(resources,
                    resourceNode->GetResourceName(),
                    get_gl_type(resourceNode->GetOutput()->GetValueType()),
                    resourceNode->GetMatType());
      }
    }
  }
  std::sort(resources.begin(), resources.end(), [](ResourceInput &a, ResourceInput &b) -> bool {
    return a.Name.compare(b.Name) < 0;
  });
  return resources;
}


void GL4ShaderGraphCompiler::GenerateVariable(SGNode *node)
{
  const Class *cls = node->GetClass();
  if (cls->IsInstanceOf<SGConstFloat>())
  {
    std::string v = VarName();
    m_nodeVariables[node] = {
        "float " + v + " = " + std::to_string(node->GetInput(0)->GetScalar()) + ";",
        v,
        eSGValueType::Float,
        false
    };
  }
  else if (cls->IsInstanceOf<SGConstVec2>())
  {
    std::string v = VarName();
    m_nodeVariables[node] = {
        "vec2 " + v + " = vec2(" + std::to_string(node->GetInput(0)->GetScalar()) + ", " +
        std::to_string(node->GetInput(1)->GetScalar()) + ");",
        v,
        eSGValueType::Vector2,
        false
    };
  }
  else if (cls->IsInstanceOf<SGConstVec3>())
  {
    std::string v = VarName();
    m_nodeVariables[node] = {
        "vec3 " + v + " = vec3(" + std::to_string(node->GetInput(0)->GetScalar()) + ", " +
        std::to_string(node->GetInput(1)->GetScalar()) + ", " +
        std::to_string(node->GetInput(2)->GetScalar()) + ");",
        v,
        eSGValueType::Vector3,
        false
    };
  }
  else if (cls->IsInstanceOf<SGConstVec4>())
  {
    std::string v = VarName();
    m_nodeVariables[node] = {
        "vec4 " + v + " = vec4(" + std::to_string(node->GetInput(0)->GetScalar()) + ", " +
        std::to_string(node->GetInput(1)->GetScalar()) + ", " +
        std::to_string(node->GetInput(2)->GetScalar()) + ", " +
        std::to_string(node->GetInput(3)->GetScalar()) + ");",
        v,
        eSGValueType::Vector4,
        false
    };
  }
  else if (cls->IsInstanceOf<SGConstColor3>())
  {
    std::string v = VarName();
    m_nodeVariables[node] = {
        "vec3 " + v + " = vec3(" + std::to_string(node->GetInput(0)->GetScalar()) + ", " +
        std::to_string(node->GetInput(1)->GetScalar()) + ", " +
        std::to_string(node->GetInput(2)->GetScalar()) + ");",
        v,
        eSGValueType::Vector3,
        false
    };
  }
  else if (cls->IsInstanceOf<SGConstColor4>())
  {
    std::string v = VarName();
    m_nodeVariables[node] = {
        "vec4 " + v + " = vec4(" + std::to_string(node->GetInput(0)->GetScalar()) + ", " +
        std::to_string(node->GetInput(1)->GetScalar()) + ", " +
        std::to_string(node->GetInput(2)->GetScalar()) + ", " +
        std::to_string(node->GetInput(3)->GetScalar()) + ");",
        v,
        eSGValueType::Vector4,
        false
    };
  }
  else if (cls->IsInstanceOf<SGVec2>())
  {
    std::string v      = VarName();
    auto        inputX = GetInputValue(node->GetInput(0));
    auto        inputY = GetInputValue(node->GetInput(1));

    if (inputX.Name == inputY.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec2 " + v + " = " + inputX.Name + "." + inputX.PostFix + inputY.PostFix + ";",
          v,
          eSGValueType::Vector2,
          false
      };

    }
    else
    {
      m_nodeVariables[node] = {
          "vec2 " + v + " = vec2(" + inputX.FullQualified() + ", " + inputY.FullQualified() + ");",
          v,
          eSGValueType::Vector2,
          false
      };
    }

  }
  else if (cls->IsInstanceOf<SGVec3>())
  {
    std::string v      = VarName();
    auto        inputX = GetInputValue(node->GetInput(0));
    auto        inputY = GetInputValue(node->GetInput(1));
    auto        inputZ = GetInputValue(node->GetInput(2));

    if (inputX.Name == inputY.Name && inputY.Name == inputZ.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec3 " + v + " = " + inputX.Name + "." + inputX.PostFix + inputY.PostFix + inputZ.PostFix + ";",
          v,
          eSGValueType::Vector3,
          false
      };
    }
    else if (inputX.Name == inputY.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec3 " + v + " = vec3(" + inputX.Name + "." + inputX.PostFix + inputY.PostFix + ", " +
          inputZ.FullQualified() + ");",
          v,
          eSGValueType::Vector3,
          false
      };
    }
    else if (inputY.Name == inputZ.Name && inputY.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec3 " + v + " = vec3(" + inputZ.FullQualified() + ", " + inputY.Name + "." + inputY.PostFix +
          inputZ.PostFix + ");",
          v,
          eSGValueType::Vector3,
          false
      };
    }
    else
    {
      m_nodeVariables[node] = {
          "vec3 " + v + " = vec3(" + inputX.FullQualified() + ", " + inputY.FullQualified() + ", " +
          inputZ.FullQualified() + ");",
          v,
          eSGValueType::Vector3,
          false
      };
    }

  }
  else if (cls->IsInstanceOf<SGVec4>())
  {
    std::string v      = VarName();
    auto        inputX = GetInputValue(node->GetInput(0));
    auto        inputY = GetInputValue(node->GetInput(1));
    auto        inputZ = GetInputValue(node->GetInput(2));
    auto        inputW = GetInputValue(node->GetInput(3));

    if (inputX.Name == inputY.Name && inputX.Name == inputZ.Name && inputX.Name == inputW.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec4 " + v + " = " + inputX.Name + "." + inputX.PostFix + inputY.PostFix + inputZ.PostFix + inputW.PostFix +
          ";",
          v,
          eSGValueType::Vector4,
          false
      };
    }
    else if (inputX.Name == inputY.Name && inputX.Name == inputZ.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec4 " + v + " = vec4(" + inputX.Name + "." + inputX.PostFix + inputY.PostFix + inputZ.PostFix + ", " +
          inputW.FullQualified() + ");",
          v,
          eSGValueType::Vector4,
          false
      };
    }
    else if (inputY.Name == inputZ.Name && inputY.Name == inputW.Name && inputY.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec4 " + v + " = vec4(" + inputX.FullQualified() + ", " + inputY.Name + "." + inputY.PostFix +
          inputZ.PostFix + inputW.PostFix + ");",
          v,
          eSGValueType::Vector4,
          false
      };
    }
    else if (inputX.Name == inputY.Name && inputZ.Name == inputW.Name && inputX.PostFix != "" && inputZ.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec4 " + v + " = vec4(" + inputX.Name + "." + inputX.PostFix + inputY.PostFix + ", " + inputZ.Name + "." +
          inputZ.PostFix + inputW.PostFix + ");",
          v,
          eSGValueType::Vector4,
          false
      };
    }
    else if (inputX.Name == inputY.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec4 " + v + " = vec4(" + inputX.Name + "." + inputX.PostFix + inputY.PostFix + ", " +
          inputZ.FullQualified() + ", " + inputW.FullQualified() + ");",
          v,
          eSGValueType::Vector4,
          false
      };
    }
    else if (inputY.Name == inputZ.Name && inputY.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec4 " + v + " = vec4(" + inputX.FullQualified() + ", " + inputY.Name + "." + inputY.PostFix +
          inputZ.PostFix + ", " + inputW.FullQualified() + ");",
          v,
          eSGValueType::Vector4,
          false
      };
    }
    else if (inputZ.Name == inputW.Name && inputZ.PostFix != "")
    {
      m_nodeVariables[node] = {
          "vec4 " + v + " = vec4(" + inputX.FullQualified() + ", " + inputY.FullQualified() + ", " + inputZ.Name + "." +
          inputZ.PostFix + inputW.PostFix + ");",
          v,
          eSGValueType::Vector4,
          false
      };
    }
    else
    {
      m_nodeVariables[node] = {
          "vec4 " + v + " = vec4(" + inputX.FullQualified() + ", " + inputY.FullQualified() + ", " +
          inputZ.FullQualified() + ", " + inputW.FullQualified() + ");",
          v,
          eSGValueType::Vector4,
          false
      };
    }
  }

  else if (cls->IsInstanceOf<SGStreamNode>())
  {
    auto *streamNode = node->Query<SGStreamNode>();
    m_nodeVariables[node] = {
        "",
        stream_name(streamNode->GetStream()),
        streamNode->GetOutput()->GetValueType(),
        true
    };
  }
  else if (cls->IsInstanceOf<SGResourceNode>())
  {
    if (cls->IsInstanceOf<SGTexture1D>())
    {
      auto        texture1D = node->Query<SGTexture1D>();
      std::string v         = VarName();
      std::string uv        = GetInputValue(texture1D->GetInput(0)).FullQualified();

      m_nodeVariables[node] = {
          "vec4 " + v + " = texture(cs_" + texture1D->GetResourceName() + ", " + uv + ");",
          v,
          texture1D->GetOutput(0)->GetValueType(),
          false
      };


    }
    else if (cls->IsInstanceOf<SGTexture2D>())
    {
      auto        texture2D = node->Query<SGTexture2D>();
      std::string v         = VarName();
      SGNodeInput *uvInput  = texture2D->GetInput(0);
      std::string uv        = uvInput->GetSource()
                              ? GetInputValue(uvInput).FullQualified()
                              : ("#STREAM-STAGE#" + stream_name(eVertexStream::eVS_UV));

      m_nodeVariables[node] = {
          "vec4 " + v + " = texture(cs_" + texture2D->GetResourceName() + ", " + uv + ");",
          v,
          texture2D->GetOutput(0)->GetValueType(),
          false
      };


    }

    else if (cls->IsInstanceOf<SGTexture3D>())
    {
      auto        texture3D = node->Query<SGTexture3D>();
      std::string v         = VarName();
      std::string uv        = GetInputValue(texture3D->GetInput(0)).FullQualified();

      m_nodeVariables[node] = {
          "vec4 " + v + " = texture(cs_" + texture3D->GetResourceName() + ", " + uv + ");",
          v,
          texture3D->GetOutput(0)->GetValueType(),
          false
      };


    }

    else
    {
      auto resource = node->Query<SGResourceNode>();
      m_nodeVariables[node] = {
          "",
          "cs_" + resource->GetResourceName(),
          resource->GetOutput(0)->GetValueType(),
          false
      };
    }
  }
  else if (cls->IsInstanceOf<SGBinaryOperator>())
  {
    auto        bin    = node->Query<SGBinaryOperator>();
    std::string v      = VarName();
    auto        inputA = GetInputValue(node->GetInput(0));
    auto        inputB = GetInputValue(node->GetInput(1));
    auto        output = node->GetOutput();

    m_nodeVariables[node] = {
        get_gl_type(output->GetValueType()) + " " + v + " = " + inputA.FullQualified() + " " +
        get_operator(bin->GetOperator()) + " " + inputB.FullQualified() + ";",
        v,
        output->GetValueType(),
        false
    };
  }
}


void GL4ShaderGraphCompiler::GenerateVariable(SGNodeOutput *output)
{
  SGNode      *node = output->GetNode();
  const Class *cls  = node->GetClass();
  auto        var   = m_nodeVariables[node];
  std::string v     = var.Name;

  std::string  name    = v;
  std::string  postfix = "";
  eSGValueType type    = var.Type;
  bool         stream  = var.Stream;


  if (cls->IsInstanceOf<SGDecomposeVec2>())
  {
    SGNodeInput  *input  = node->GetInput(0);
    SGNodeOutput *source = input->GetSource();
    if (m_outputVariables.contains(source))
    {
      OutputVariable &variable = m_outputVariables[source];
      name   = variable.FullQualified();
      stream = variable.Stream;
      type   = eSGValueType::Float;

      if (output->GetName() == SGDecomposeVec2::OUT_X)
      {
        postfix = "x";
      }
      else if (output->GetName() == SGDecomposeVec2::OUT_Y)
      {
        postfix = "y";
      }
    }
  }
  else if (cls->IsInstanceOf<SGDecomposeVec3>())
  {
    SGNodeInput  *input  = node->GetInput(0);
    SGNodeOutput *source = input->GetSource();
    if (m_outputVariables.contains(source))
    {
      OutputVariable &variable = m_outputVariables[source];
      name   = variable.FullQualified();
      stream = variable.Stream;
      type   = eSGValueType::Float;

      if (output->GetName() == SGDecomposeVec3::OUT_X)
      {
        postfix = "x";
      }
      else if (output->GetName() == SGDecomposeVec3::OUT_Y)
      {
        postfix = "y";
      }
      else if (output->GetName() == SGDecomposeVec3::OUT_Z)
      {
        postfix = "z";
      }
    }
  }
  else if (cls->IsInstanceOf<SGDecomposeVec4>())
  {
    SGNodeInput  *input  = node->GetInput(0);
    SGNodeOutput *source = input->GetSource();
    if (m_outputVariables.contains(source))
    {
      OutputVariable &variable = m_outputVariables[source];
      name   = variable.FullQualified();
      stream = variable.Stream;
      type   = eSGValueType::Float;

      if (output->GetName() == SGDecomposeVec4::OUT_X)
      {
        postfix = "x";
      }
      else if (output->GetName() == SGDecomposeVec4::OUT_Y)
      {
        postfix = "y";
      }
      else if (output->GetName() == SGDecomposeVec4::OUT_Z)
      {
        postfix = "z";
      }
      else if (output->GetName() == SGDecomposeVec4::OUT_W)
      {
        postfix = "w";
      }
    }
  }


  m_outputVariables[output] = {name, postfix, type, stream};
}


GL4ShaderGraphCompiler::OutputVariable GL4ShaderGraphCompiler::GetInputValue(SGNodeInput *input)
{
  auto source = input->GetSource();
  if (source)
  {
    if (m_outputVariables.contains(source))
    {
      return m_outputVariables[source];
    }
  }
  return {
      std::to_string(input->GetScalar()),
      "",
      eSGValueType::Float,
      false
  };
}

std::string GL4ShaderGraphCompiler::VarName()
{
  size_t idx = m_nextVariableName;
  m_nextVariableName++;

  return "v_" + std::to_string(idx);
}


static void string_replace(std::string &str, const std::string &oldStr, const std::string &newStr)
{
  std::string::size_type pos = 0u;
  while ((pos = str.find(oldStr, pos)) != std::string::npos)
  {
    str.replace(pos, oldStr.length(), newStr);
    pos += newStr.length();
  }
}
std::string GL4ShaderGraphCompiler::NodeVariable::StagedDecl(const std::string &stage)
{
  std::string decl = Decl;
  string_replace(decl, "#STREAM-STAGE#", stage);
  return decl;
}

std::string GL4ShaderGraphCompiler::OutputVariable::FullQualified()
{
  if (PostFix.empty())
  {
    return StagedName();
  }
  return StagedName() + "." + PostFix;
}

std::string GL4ShaderGraphCompiler::OutputVariable::StagedName()
{
  if (Stream)
  {
    return "#STREAM-STAGE#" + Name;
  }
  return Name;
}

}