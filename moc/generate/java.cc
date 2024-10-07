#include <generate/java.hh>
#include <ast.hh>
#include <generate/generator.hh>
#include <generate/javaconverter.hh>

namespace cs::moc
{

std::string JavaGenerator::OutputClass(ClassNode *classNode, std::list<NamespaceNode *> &nss, CSMetaNode *classMeta)
{
  std::string source;
  if (!classMeta->Has("jclass"))
  {
    return source;
  }

  source += "#ifdef CS_JAVA\n\n";
  source += "#include <jni.h>\n";
  source += "#include <csCore/csClass.hh>\n\n";

  std::string javaFQClass = classMeta->Get("jclass");

  ASTNode *childBlock = classNode->GetChildren().size() >= 1 ? classNode->GetChildren()[0] : nullptr;
  if (!childBlock || childBlock->GetType() != eANT_Block)
  {
    return source;
  }


  CSMetaNode *lastMeta = nullptr;
  for (auto  child: childBlock->GetChildren())
  {
    if (child->GetType() == eANT_CSMeta)
    {
      lastMeta = static_cast<CSMetaNode *>(child);
    }
    else if (child->GetType() == eANT_Function)
    {
      if (lastMeta && lastMeta->GetMetaType() == CSMetaNode::eMT_Function)
      {
        source += GenerateFunction(classNode, nss, classMeta, reinterpret_cast<FunctionNode *>(child), lastMeta);
        lastMeta = nullptr;
      }
    }
  }

  source += "#endif CS_JAVA\n\n";

  return source;
}

static std::string convert_to_jni_method_name(const std::string &javaFQClass, const std::string &methodName)
{
  std::string name = javaFQClass;

  if (name[0] == '"')
  {
    name = name.substr(1);
  }

  if (name[name.length() - 1] == '"')
  {
    name = name.substr(0, name.length() - 1);
  }


  while (true)
  {
    std::size_t idx = name.find('.', 0);
    if (idx == std::string::npos)
    {
      break;
    }

    name = name.replace(idx, 1, 1, '_');
  }


  return "Java_" + name + "_n" + methodName;
}


std::string convert_return_type_to_jni_type(FunctionNode *function, CSMetaNode *meta)
{
  const TypeDef       &def       = function->GetReturnValue();
  std::string         typeName   = def.GetTypeName();
  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    return converter->GetOutputReturnType();
  }

  if (def.IsPointer())
  {
    if (meta->Has("jrawPtr"))
    {
      return "jlong";
    }
    else
    {
      return "jobject";
    }
  }
  else if (def.IsReference())
  {
    return "jlong";
  }


  return "void";
}


std::string
convert_function_input_parameter_to_jni_type(const Argument &argument,
                                             CSMetaNode *meta,
                                             uint32_t jniArgCount,
                                             uint32_t &numJniArgCount)
{
  const TypeDef &def     = argument.GetType();
  std::string   typeName = def.GetTypeName();
  numJniArgCount = 1;
  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    numJniArgCount = converter->GetInputArguments().size();
    std::string     arguments;
    for (const auto &arg: converter->GetInputArguments())
    {
      arguments += ", " + arg.GetJType() + " jniArg" + std::to_string(jniArgCount++);
    }
    return arguments;
  }

  if (def.IsPointer() || def.IsReference())
  {
    return ", jlong jniArg" + std::to_string(jniArgCount);
  }
  return "";
}

std::string
convert_function_output_parameter_to_jni_type(const FunctionNode *function, CSMetaNode *meta)
{
  const TypeDef       &def        = function->GetReturnValue();
  std::string         typeName    = def.GetTypeName();
  size_t              jniArgCount = 0;
  const JavaConverter *converter  = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    std::string     arguments;
    for (const auto &arg: converter->GetOutputArguments())
    {
      arguments += ", " + arg.GetJType() + " jniOutArg" + std::to_string(jniArgCount++);
    }
    return arguments;
  }
  return "";
}


std::string
convert_input_parameter_to_jni(const Argument &argument,
                               uint32_t jniArgCount,
                               uint32_t csMethodArgCount,
                               uint32_t &numCsMethodArgCount)
{
  const TypeDef &def     = argument.GetType();
  std::string   typeName = def.GetTypeName();
  numCsMethodArgCount = 1;
  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    return converter->ConvertInput(jniArgCount, csMethodArgCount);
  }

  if (def.IsPointer())
  {
    return def.GetText() + " csArg" + std::to_string(csMethodArgCount)
           + " = reinterpret_cast<" + def.GetText() + ">(jniArg" + std::to_string(jniArgCount) + ");";
  }
  else if (def.IsReference())
  {
    return def.GetText() + " csArg" + std::to_string(csMethodArgCount)
           + " = *reinterpret_cast<" + def.GetText() + "*>(jniArg" + std::to_string(jniArgCount) + ");";
  }

  return "";
}


std::string convert_result(const FunctionNode *function, CSMetaNode *functionMeta)
{
  const TypeDef &def     = function->GetReturnValue();
  std::string   typeName = def.GetTypeName();

  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    return converter->ConvertOutput();
  }

  if (def.IsPointer())
  {
    if(functionMeta->Has("jrawPtr"))
    {
      return "return reinterpret_cast<jlong>(csReturnValue);";
    }
    else
    {
      return "return csReturnValue ? reinterpret_cast<cs::iObject*>(csReturnValue)->GetJObject() : nullptr;";
    }
  }
  else if (def.IsReference())
  {
    return "return reinterpret_cast<jlong>(&csReturnValue);";
  }
  return "";
}


std::string JavaGenerator::GenerateFunction(ClassNode *classNode, std::list<NamespaceNode *> &nss, CSMetaNode *clsMeta,
                                            FunctionNode *functionNode, CSMetaNode *functionMeta)
{
  std::string source;
  if (functionMeta->Has("noJava"))
  {
    return source;
  }

  std::string javaFQClass = clsMeta->Get("jclass");
  std::string fns         = Generator::GetFullNamespaceName(nss);

  std::string methodName = convert_to_jni_method_name(javaFQClass, functionNode->GetName());


  std::string argConversion;
  std::string functionArguments;
  uint32_t    jniArgCount      = 0;
  uint32_t    csMethodArgCount = 0;
  for (auto   &arg: functionNode->GetArguments())
  {
    uint32_t numJniArgCount, numCsMethodArgCount;
    functionArguments += convert_function_input_parameter_to_jni_type(arg, functionMeta, jniArgCount, numJniArgCount);
    argConversion += convert_input_parameter_to_jni(arg, jniArgCount, csMethodArgCount, numCsMethodArgCount);


    jniArgCount += numJniArgCount;
    csMethodArgCount += numCsMethodArgCount;
  }

  functionArguments += convert_function_output_parameter_to_jni_type(functionNode, functionMeta);

  source += "JNIEXPORT " + convert_return_type_to_jni_type(functionNode, functionMeta) + "\n";
  source += "JNICALL " + methodName + "(JNIEnv* env, jclass cls, jlong ref" + functionArguments + ")\n"
            + "{\n"
            + "  auto csRef = reinterpret_cast<" + fns + classNode->GetName() + "*>(ref);\n"
            + argConversion + "\n"
            + "  ";
  if (!functionNode->GetReturnValue().IsVoid())
  {
    source += functionNode->GetReturnValue().GetText() + " csReturnValue = ";
  }
  source += "csRef->" + functionNode->GetName() + "(";
  for (int i = 0; i < functionNode->GetArguments().size(); ++i)
  {
    if (i != 0)
    {
      source += ", ";
    }
    source += "csArg" + std::to_string(i);
  }
  source += ");\n";
  if (!functionNode->GetReturnValue().IsVoid())
  {
    source += convert_result(functionNode, functionMeta) + "\n";
  }
  source += "}\n\n";


  return source;
}


}