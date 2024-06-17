
#include <ceOpenGL/gl4/shadergraph/gl4shadergraphcompiler.hh>
#include <ceCore/graphics/shadergraph/sgnodes.hh>

namespace ce::opengl
{

void GL4ShaderGraphCompiler::GenerateVariables()
{
  for (auto node : m_linearizedNodes)
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
  case eSGValueType::Invalid: return "int";
  case eSGValueType::Float: return "float";
  case eSGValueType::Vector2: return "vec2";
  case eSGValueType::Vector3: return "vec3";
  case eSGValueType::Vector4: return "vec4";
  }
  return "";
}

std::string get_operator(SGBinaryOperator::eOperator op)
{
  switch (op)
  {
  case SGBinaryOperator::Add: return "+";
  case SGBinaryOperator::Sub: return "-";
  case SGBinaryOperator::Mul: return "*";
  case SGBinaryOperator::Div: return "/";
  case SGBinaryOperator::Mod: return "%";
  }
  return "";
}


void GL4ShaderGraphCompiler::GenerateVariable(SGNode* node)
{
  const Class* cls = node->GetClass();
  if (cls->IsInstanceOf<SGConstFloat>())
  {
    std::string v = VarName();
    m_nodeVariables[node] = {
      "float" + v + " = " + std::to_string(node->GetInput(0)->GetScalar()) + ");",
      v,
      eSGValueType::Float
    };
  }
  else if (cls->IsInstanceOf<SGConstVec2>())
  {
    std::string v = VarName();
    m_nodeVariables[node] = {
      "vec2 " + v + " = vec2(" + std::to_string(node->GetInput(0)->GetScalar()) + ", " +
                                 std::to_string(node->GetInput(1)->GetScalar()) + ");",
      v,
      eSGValueType::Vector2
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
      eSGValueType::Vector3
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
      eSGValueType::Vector4
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
      eSGValueType::Vector3
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
      eSGValueType::Vector4
    };
  }
  else if (cls->IsInstanceOf<SGVec3>())
  {
    std::string v = VarName();
    auto inputX = GetInputValue(node->GetInput(0));
    auto inputY = GetInputValue(node->GetInput(1));
    auto inputZ = GetInputValue(node->GetInput(2));

    if (inputX.Name == inputY.Name && inputY.Name == inputZ.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec3 " + v + " = " + inputX.Name + "." + inputX.PostFix + inputY.PostFix + inputZ.PostFix + ";",
        v,
        eSGValueType::Vector3
      };
    }
    else if (inputX.Name == inputY.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec3 " + v + " = vec3(" + inputX.Name + "." + inputX.PostFix + inputY.PostFix + ", " + inputZ.FullQuallified + ");",
        v,
        eSGValueType::Vector3
      };
    }
    else if (inputY.Name == inputZ.Name && inputY.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec3 " + v + " = vec3(" + inputZ.FullQuallified + ", " + inputY.Name + "." + inputY.PostFix + inputZ.PostFix + ");",
        v,
        eSGValueType::Vector3
      };
    }
    else
    {
      m_nodeVariables[node] = {
        "vec3 " + v + " = vec3(" + inputX.FullQuallified + ", " + inputY.FullQuallified + ", " + inputZ.FullQuallified + ");",
        v,
        eSGValueType::Vector3
      };
    }

  }
  else if (cls->IsInstanceOf<SGVec4>())
  {
    std::string v = VarName();
    auto inputX = GetInputValue(node->GetInput(0));
    auto inputY = GetInputValue(node->GetInput(1));
    auto inputZ = GetInputValue(node->GetInput(2));
    auto inputW = GetInputValue(node->GetInput(3));

    if (inputX.Name == inputY.Name && inputX.Name == inputZ.Name && inputX.Name == inputW.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec4 " + v + " = " + inputX.Name + "." + inputX.PostFix + inputY.PostFix + inputZ.PostFix + inputW.PostFix + ";",
        v,
        eSGValueType::Vector4
      };
    }
    else if (inputX.Name == inputY.Name && inputX.Name == inputZ.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec4 " + v + " = vec4(" + inputX.Name + "." + inputX.PostFix + inputY.PostFix + inputZ.PostFix + ", "  + inputW.FullQuallified+ ");",
        v,
        eSGValueType::Vector4
      };
    }
    else if (inputY.Name == inputZ.Name && inputY.Name == inputW.Name && inputY.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec4 " + v + " = vec4(" +inputX.FullQuallified + ", " + inputY.Name + "." + inputY.PostFix + inputZ.PostFix + inputW.PostFix + ");",
        v,
        eSGValueType::Vector4
      };
    }
    else if (inputX.Name == inputY.Name && inputZ.Name == inputW.Name && inputX.PostFix != "" && inputZ.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec4 " + v + " = vec4(" + inputX.Name + "." + inputX.PostFix + inputY.PostFix + ", " + inputZ.Name + "." +  inputZ.PostFix + inputW.PostFix + ");",
        v,
        eSGValueType::Vector4
      };
    }
    else if (inputX.Name == inputY.Name && inputX.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec4 " + v + " = vec4(" + inputX.Name + "." + inputX.PostFix + inputY.PostFix + ", " + inputZ.FullQuallified + ", " + inputW.FullQuallified + ");",
        v,
        eSGValueType::Vector4
      };
    }
    else if (inputY.Name == inputZ.Name && inputY.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec4 " + v + " = vec4(" + inputX.FullQuallified + ", " + inputY.Name + "." + inputY.PostFix + inputZ.PostFix + ", " + inputW.FullQuallified + ");",
        v,
        eSGValueType::Vector4
      };
    }
    else if (inputZ.Name == inputW.Name && inputZ.PostFix != "")
    {
      m_nodeVariables[node] = {
        "vec4 " + v + " = vec4(" + inputX.FullQuallified + ", " + inputY.FullQuallified + ", " + inputZ.Name + "." + inputZ.PostFix + inputW.PostFix + ");",
        v,
        eSGValueType::Vector4
      };
    }
    else

    {
      m_nodeVariables[node] = {
        "vec4 " + v + " = vec4(" + inputX.FullQuallified + ", " + inputY.FullQuallified + ", " + inputZ.FullQuallified + ", " + inputW.FullQuallified + ");",
        v,
        eSGValueType::Vector4
      };
    }
  }
  else if (cls->IsInstanceOf<SGBinaryOperator>())
  {
    auto bin = node->Query<SGBinaryOperator>();
    std::string v = VarName();
    auto inputA = GetInputValue(node->GetInput(0));
    auto inputB = GetInputValue(node->GetInput(1));
    auto output = node->GetOutput();

    m_nodeVariables[node] = {
      get_gl_type(output->GetValueType()) + " " + v + " = " + inputA.FullQuallified + " " + get_operator(bin->GetOperator()) + " " + inputB.FullQuallified + ";",
      v,
      output->GetValueType()
    };
  }
}


void GL4ShaderGraphCompiler::GenerateVariable(SGNodeOutput* output)
{
  SGNode* node = output->GetNode();
  const Class* cls = node->GetClass();
  auto var = m_nodeVariables[node];
  std::string v = var.Name;

  std::string res_v = v;
  std::string res_p = "";
  std::string res_f = v;
  eSGValueType res_t = var.Type;
  eSGValueType res_ft = var.Type;

  if (cls->IsInstanceOf<SGConstVec2>())
  {
    if (output->GetName() == SGConstVec2::OUT_X)
    {
      res_f = v + ".x";
      res_p = "x";
      res_ft = eSGValueType::Float;
    }
    else if (output->GetName() == SGConstVec2::OUT_Y)
    {
      res_f = v + ".y";
      res_p = "y";
      res_ft = eSGValueType::Float;
    }
  }
  else if (cls->IsInstanceOf<SGConstColor4>())
  {
    if (output->GetName() == SGConstColor4::OUT_R)
    {
      res_f = v + ".r";
      res_p = "r";
      res_ft = eSGValueType::Float;
    }
    else if (output->GetName() == SGConstColor4::OUT_G)
    {
      res_f = v + ".g";
      res_p = "g";
      res_ft = eSGValueType::Float;
    }
    else if (output->GetName() == SGConstColor4::OUT_B)
    {
      res_f = v + ".b";
      res_p = "b";
      res_ft = eSGValueType::Float;
    }
    else if (output->GetName() == SGConstColor4::OUT_A)
    {
      res_f = v + ".a";
      res_p = "a";
      res_ft = eSGValueType::Float;
    }
  }
  else if (cls->IsInstanceOf<SGDecomposeVec2>())
  {
    SGNodeInput *input = node->GetInput(0);
    SGNodeOutput *source = input->GetSource();
    if (m_outputVariables.contains(source))
    {
      OutputVariable &variable = m_outputVariables[source];
      res_v = variable.FullQuallified;
      res_t = variable.FullQuallifiedType;
      res_ft = eSGValueType::Float;

      if (output->GetName() == SGDecomposeVec2::OUT_X)
      {
        res_p = "x";
        res_f = variable.FullQuallified + ".x";
      }
      else if (output->GetName() == SGDecomposeVec2::OUT_Y)
      {
        res_p = "y";
        res_f = variable.FullQuallified + ".y";
      }
    }
  }


  m_outputVariables[output] = { res_v, res_p, res_f, res_t, res_ft };

}


GL4ShaderGraphCompiler::OutputVariable GL4ShaderGraphCompiler::GetInputValue(SGNodeInput* input)
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
    std::to_string(input->GetScalar()),
    eSGValueType::Float
  };
}





std::string GL4ShaderGraphCompiler::VarName()
{
  size_t idx = m_nextVariableName;
  m_nextVariableName++;

  return "v_" + std::to_string(idx);
}

}