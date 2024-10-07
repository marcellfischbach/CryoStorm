#include <generate/java.hh>
#include <ast.hh>
#include <generate/generator.hh>
#include <generate/javaconverter.hh>

namespace cryo::moc
{

std::string JavaGenerator::OutputClass(ClassNode *classNode, std::list<NamespaceNode *> &nss, CSMetaNode *classMeta)
{
  std::string source;
  if (!classMeta->Has("jclass"))
  {
    return source;
  }

  source += "#ifdef CS_JAVA\n\n";
  source += "#include <jni.h>\n\n";

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

  if (typeName == "std::string" || typeName == "string")
  {
    return "jstring";
  }
  else if (typeName == "int8_t")
  {
    return "jbyte";
  }
  else if (typeName == "uint8_t" || typeName == "int16_t")
  {
    return "jshort";
  }
  else if (typeName == "int" || typeName == "uint16_t" || typeName == "int32_t")
  {
    return "jint";
  }
  else if (typeName == "unsigned" || typeName == "uint32_t" || typeName == "int64_t")
  {
    return "jlong";
  }
  else if (typeName == "float")
  {
    return "jfloat";
  }
  else if (typeName == "double")
  {
    return "jdouble";
  }

  if (def.IsPointer() || def.IsPointerToPointer() || def.IsReference())
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

  if (typeName == "std::string" || typeName == "string")
  {
    return ", jstring arg" + std::to_string(jniArgCount);
  }
  else if (typeName == "int8_t")
  {
    return ", jbyte arg" + std::to_string(jniArgCount);
  }
  else if (typeName == "uint8_t" || typeName == "int16_t")
  {
    return ", jshort arg" + std::to_string(jniArgCount);
  }
  else if (typeName == "int" || typeName == "uint16_t" || typeName == "int32_t")
  {
    return ", jint arg" + std::to_string(jniArgCount);
  }
  else if (typeName == "unsigned" || typeName == "uint32_t" || typeName == "int64_t")
  {
    return ", jlong arg" + std::to_string(jniArgCount);
  }
  else if (typeName == "float")
  {
    return ", jfloat arg" + std::to_string(jniArgCount);
  }
  else if (typeName == "double")
  {
    return ", jdouble arg" + std::to_string(jniArgCount);
  }


  return ", jlong arg" + std::to_string(jniArgCount);
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

std::string convert_string_input_to_jni(uint32_t inputC, uint32_t outputC)
{
  std::string argIn  = std::string("arg") + std::to_string(inputC);
  std::string argTmp = std::string("constChar") + std::to_string(outputC);
  std::string argOut = std::string("csArg") + std::to_string(outputC);
  std::string source;
  source += std::string("  const char *") + argTmp + " = env->GetStringUTFChars(" + argIn + ", 0);\n";
  source += std::string("  std::string ") + argOut + "(" + argTmp + ");\n";
  source += std::string("  env->ReleaseStringUTFChars(" + argIn + ", " + argTmp + ");\n");

  return source;
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
  if (typeName == "std::string" || typeName == "string")
  {
    return convert_string_input_to_jni(jniArgCount, csMethodArgCount);
  }
  else if (typeName == "int8_t")
  {
    return ", jbyte arg" + std::to_string(jniArgCount++);
  }
  else if (typeName == "uint8_t" || typeName == "int16_t")
  {
    return ", jshort arg" + std::to_string(jniArgCount++);
  }
  else if (typeName == "int" || typeName == "uint16_t" || typeName == "int32_t")
  {
    return ", jint arg" + std::to_string(jniArgCount++);
  }
  else if (typeName == "unsigned" || typeName == "uint32_t" || typeName == "int64_t")
  {
    return ", jlong arg" + std::to_string(jniArgCount++);
  }
  else if (typeName == "float")
  {
    return ", jfloat arg" + std::to_string(jniArgCount++);
  }
  else if (typeName == "double")
  {
    return ", jdouble arg" + std::to_string(jniArgCount++);
  }

  return "";
}


std::string convert_result(const FunctionNode *function)
{
  const TypeDef &def     = function->GetReturnValue();
  std::string   typeName = def.GetTypeName();

  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter)
  {
    return converter->ConvertOutput();
  }

  if (typeName == "std::string" || typeName == "string")
  {
    return "  return env->NewStringUTF(csReturnValue.c_str());";
  }
  else if (typeName == "int8_t")
  {
    return " return static_cast<jbyte>(csRes);";
  }
  else if (typeName == "uint8_t" || typeName == "int16_t")
  {
    return " return static_cast<jshort>(csRes);";
  }
  else if (typeName == "int" || typeName == "uint16_t" || typeName == "int32_t")
  {
    return " return static_cast<jint>(csRes);";
  }
  else if (typeName == "unsigned" || typeName == "uint32_t" || typeName == "int64_t")
  {
    return " return static_cast<jlong>(csRes);";
  }
  else if (typeName == "float")
  {
    return " return static_cast<jfloat>(csRes);";
  }
  else if (typeName == "double")
  {
    return " return static_cast<jdouble>(csRes);";
  }


  return "";
}

std::string convert_return_type_to_cs(const FunctionNode *function)
{
  const TypeDef &def     = function->GetReturnValue();
  std::string   typeName = def.GetTypeName();

  const JavaConverter *converter = JavaConverters::Get()->FindConverter(typeName);
  if (converter && !converter->GetFullQualifiedType().empty())
  {
    std::string fullTypeName;
    if (def.IsConst())
    {
      fullTypeName += "const ";
    }
    fullTypeName += converter->GetFullQualifiedType();
    if (def.IsReference())
    {
      fullTypeName += "&";
    }
    else if (def.IsPointerToPointer())
    {
      fullTypeName += "**";
    }
    else if (def.IsPointer())
    {
      fullTypeName += "*";
    }
    return fullTypeName;
  }
  return function->GetReturnValue().GetText();
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
    source += convert_return_type_to_cs(functionNode) + " csReturnValue = ";
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
    source += convert_result(functionNode) + "\n";
  }
  source += "}\n\n";


  return source;
}


}