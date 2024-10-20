#include <ast.hh>
#include <generate/generator.hh>
#include <generate/javaconverter.hh>
#include <generate/JavaSourceGenerator.hh>
#include <generate/JavaJNIGenerator.hh>

namespace cs::moc
{

std::string JavaJNIGenerator::OutputClass(ClassNode *classNode, std::list<NamespaceNode *> &nss, CSMetaNode *classMeta)
{
  if (!classMeta->Has("jclass"))
  {
    return "";
  }

  std::string fns = Generator::GetFullNamespaceName(nss);
  std::string className = classNode->GetName();

  std::string javaFQClass = classMeta->Get("jclass");
  m_sourceGenerator.BeginClass(classNode, classMeta, fns + className, javaFQClass);

  std::string source;
  source += "#ifdef CS_JAVA\n\n";
  source += "#include <jni.h>\n";
  source += "#include <csCore/csClass.hh>\n\n";


  ASTNode *childBlock = classNode->GetChildren().size() >= 1 ? classNode->GetChildren()[0] : nullptr;
  if (!childBlock || childBlock->GetType() != eANT_Block)
  {
    return source;
  }


  CSMetaNode *lastMeta = nullptr;
  for (auto child: childBlock->GetChildren())
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

  m_sourceGenerator.EndClass();

  source += "#endif // CS_JAVA\n\n";

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


  return "Java_" + name + "Native_n" + methodName;
}


std::string convert_return_type_to_jni_type(FunctionNode *function, CSMetaNode *meta)
{
  const TypeDef &def = function->GetReturnValue();
  std::string typeName = def.GetTypeName();
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

  if (function->GetArguments().empty() && meta->Has("jenum")
      || meta->HasListValue("jenum", typeName))
  {
    return "jint";
  }

  return "void";
}


std::string
convert_function_input_parameter_to_jni_type(const FunctionNode *function,
                                             const CSMetaNode *meta,
                                             const Argument &argument,
                                             uint32_t jniArgCount,
                                             uint32_t &numJniArgCount,
                                             JavaSourceGenerator &sourceGenerator)
{
  const TypeDef &def = argument.GetType();
  std::string typeName = def.GetTypeName();
  std::string prettyTypeName = def.GetText();
  numJniArgCount = 1;
  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    numJniArgCount = converter->GetInputArguments().size();
    std::string arguments;
    for (const auto &arg: converter->GetInputArguments())
    {
      sourceGenerator.AddParameter(arg.GetJType(),
                                   argument.GetName() + arg.GetSuffix(),
                                   argument.GetName() + " (" + prettyTypeName + ")");
      arguments += ", " + arg.GetJType() + " jniArg" + std::to_string(jniArgCount);
      jniArgCount++;
    }
    return arguments;
  }

  if (def.IsPointer() || def.IsReference())
  {
    sourceGenerator.AddParameter("jlong",
                                 argument.GetName(),
                                 argument.GetName() + " (" + prettyTypeName + ")");
    return ", jlong jniArg" + std::to_string(jniArgCount);
  }

  if (function->GetArguments().size() == 1 && meta->Has("jenum")
    || meta->HasListValue("jenum", typeName))
  {
    sourceGenerator.AddParameter("jint",
                                 argument.GetName(),
                                 argument.GetName() + " (" + prettyTypeName + ")");
    return ", jint jniArg" + std::to_string(jniArgCount);
  }

  return "";
}

std::string
convert_function_output_parameter_to_jni_type(const FunctionNode *function, CSMetaNode *meta,
                                              JavaSourceGenerator &sourceGenerator)
{
  const TypeDef &def = function->GetReturnValue();
  std::string typeName = def.GetTypeName();
  size_t jniArgCount = 0;
  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    std::string arguments;
    for (const auto &arg: converter->GetOutputArguments())
    {
      sourceGenerator.AddParameter(arg.GetJType(), "outArg" + std::to_string(jniArgCount),
                                   typeName + " (output return value)");
      arguments += ", " + arg.GetJType() + " jniOutArg" + std::to_string(jniArgCount);
      jniArgCount++;
    }
    return arguments;
  }
  return "";
}


std::string
convert_input_parameter_to_jni(FunctionNode *functionNode,
                               CSMetaNode *functionMeta,
                               const Argument &argument,
                               uint32_t jniArgCount,
                               uint32_t csMethodArgCount,
                               uint32_t &numCsMethodArgCount)
{
  const TypeDef &def = argument.GetType();
  std::string typeName = def.GetTypeName();
  numCsMethodArgCount = 1;
  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    return converter->ConvertInput(jniArgCount, csMethodArgCount);
  }

  if (def.IsPointer())
  {
    return def.GetText() + " csArg" + std::to_string(csMethodArgCount)
           + " = reinterpret_cast<" + def.GetText() + ">(jniArg" + std::to_string(jniArgCount) + ");\n";
  }
  else if (def.IsReference())
  {
    return def.GetText() + " csArg" + std::to_string(csMethodArgCount)
           + " = *reinterpret_cast<" + def.GetText() + "*>(jniArg" + std::to_string(jniArgCount) + ");\n";
  }
  if (functionNode->GetArguments().size() == 1 && functionMeta->Has("jenum")
      || functionMeta->HasListValue("jenum", typeName))
  {
    return def.GetText() + " csArg" + std::to_string(csMethodArgCount)
           + " = static_cast<" + def.GetText() + ">(jniArg" + std::to_string(jniArgCount) + ");\n";
  }

  return "";
}


std::string convert_result(const FunctionNode *function, CSMetaNode *functionMeta)
{
  const TypeDef &def = function->GetReturnValue();
  std::string typeName = def.GetTypeName();

  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    return converter->ConvertOutput();
  }

  if (def.IsPointer())
  {
    if (functionMeta->Has("jrawPtr"))
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

  if (function->GetArguments().empty() && functionMeta->Has("jenum")
      || functionMeta->HasListValue("jenum", typeName))
  {
    return "return static_cast<jint>(csReturnValue);";
  }
  return "";
}


std::string
JavaJNIGenerator::GenerateFunction(ClassNode *classNode, std::list<NamespaceNode *> &nss, CSMetaNode *clsMeta,
                                   FunctionNode *functionNode, CSMetaNode *functionMeta)
{
  std::string source;
  if (functionMeta->Has("noJava"))
  {
    return source;
  }

  std::string javaFQClass = clsMeta->Get("jclass");
  std::string fns = Generator::GetFullNamespaceName(nss);

  std::string methodName = convert_to_jni_method_name(javaFQClass, functionNode->GetName());
  const std::string &jniReturnType = convert_return_type_to_jni_type(functionNode, functionMeta);
  m_sourceGenerator.BeginFunction(functionNode, functionMeta, functionNode->GetName(), jniReturnType);


  std::string argConversion;
  std::string functionArguments;
  uint32_t jniArgCount = 0;
  uint32_t csMethodArgCount = 0;
  for (auto &arg: functionNode->GetArguments())
  {
    uint32_t numJniArgCount, numCsMethodArgCount;
    functionArguments += convert_function_input_parameter_to_jni_type(functionNode,
                                                                      functionMeta,
                                                                      arg,
                                                                      jniArgCount,
                                                                      numJniArgCount,
                                                                      m_sourceGenerator);
    argConversion += convert_input_parameter_to_jni(functionNode,
                                                    functionMeta,
                                                    arg,
                                                    jniArgCount,
                                                    csMethodArgCount,
                                                    numCsMethodArgCount);


    jniArgCount += numJniArgCount;
    csMethodArgCount += numCsMethodArgCount;
  }

  functionArguments += convert_function_output_parameter_to_jni_type(functionNode, functionMeta, m_sourceGenerator);

  source += "JNIEXPORT " + jniReturnType + "\n";
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

  m_sourceGenerator.EndFunction();

  return source;
}


}