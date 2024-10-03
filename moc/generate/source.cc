
#include <generate/source.hh>
#include <generate/ioutput.hh>
#include <ast.hh>


namespace ce::moc
{

SourceGenerator::SourceGenerator()
    : Generator()
{

}

void SourceGenerator::Output(iOutput *output)
{

  std::vector<ClassNode *> classes = FindAllMajorClasses();

  std::string source;


  for (auto cls: classes)
  {
    ASTNode *prev = cls->FindPrevSibling();
    if (prev == nullptr || prev->GetType() != eANT_CSMeta)
    {
      continue;
    }

    CSMetaNode *meta = static_cast<CSMetaNode *>(prev);
    if (meta->GetMetaType() != CSMetaNode::eMT_Class)
    {
      continue;
    }

    std::list<NamespaceNode *> nss = GetAllNamespaces(cls);


    ClassGenerator classGenerator;
    source += classGenerator.OutputClass(cls, nss, meta);


  }


  if (!classes.empty() && output)
  {
    output->output(source);
  }
}


std::string ClassGenerator::OutputClass(ClassNode *classNode, std::list<NamespaceNode *> &nss, CSMetaNode *meta)
{
  std::string source;

  ASTNode *childBlock = classNode->GetChildren().size() >= 1 ? classNode->GetChildren()[0] : nullptr;
  if (!childBlock || childBlock->GetType() != eANT_Block)
  {
    return source;
  }

  std::string visibility = "private";

  CSMetaNode *lastMeta = nullptr;
  for (auto  child: childBlock->GetChildren())
  {
    if (child->GetType() == eANT_Visibility)
    {
      visibility = static_cast<VisibilityNode *>(child)->GetVisibility();
    }
    else if (child->GetType() == eANT_CSMeta)
    {
      lastMeta = static_cast<CSMetaNode *>(child);
    }
    else if (child->GetType() == eANT_Member)
    {
      if (lastMeta && lastMeta->GetMetaType() == CSMetaNode::eMT_Property)
      {
        source += GenerateAttribute(classNode, nss, static_cast<MemberNode *>(child), visibility, lastMeta);
      }
    }
    else if (child->GetType() == eANT_Function)
    {
      if (lastMeta && lastMeta->GetMetaType() == CSMetaNode::eMT_Function)
      {
        source += GenerateFunctionClass(classNode, nss, static_cast<FunctionNode *>(child), visibility, lastMeta);
      }
    }


    if (child->GetType() != eANT_CSMeta)
    {
      lastMeta = nullptr;
    }
  }

  source += GenerateClass(classNode, nss, meta);


  return source;
}

std::string GetValueDeclarationMem(TypeDef &typeDef)
{
  if (typeDef.IsReference())
  {
    return "ce::eVMM_Reference";
  }
  else if (typeDef.IsPointer())
  {
    return "ce::eVMM_Pointer";
  }
  else if (typeDef.IsPointerToPointer())
  {
    return "ce::eVMM_PointerToPointer";
  }
  return "ce::eVMM_Value";
}

std::string ClassGenerator::GenerateFunctionClass(ClassNode *classNode,
                                                  std::list<NamespaceNode *> &nss,
                                                  FunctionNode *function,
                                                  const std::string &,
                                                  CSMetaNode *)
{
  std::string fnctClassName = classNode->GetName() + "Class_Function_" + function->GetName();
  if (function->IsConst())
  {
    fnctClassName += "_Const";
  }

  m_functionClasses.push_back(fnctClassName);

  std::string source = "";

  for (auto ns: nss)
  {
    source += "namespace " + ns->GetName() + "\n";
    source += "{\n";
  }

  source += "\nclass " + fnctClassName + " : public ce::Function\n";
  source += "{\n";
  source += "public:\n";

  // create the constructor
  source += "  " + fnctClassName + "()\n";
  source += std::string("    : ce::Function (") +
            (function->IsVirtual() ? "ce::eFV_Virtual, " : "ce::eFV_NonVirtual, ") +
            GenerateCSValueDeclaration(function->GetReturnValue()) + ", " +
            "\"" + function->GetName() + "\", " +
            (function->IsConst() ? "ce::eC_Const" : "ce::eC_NonConst") +
            ")\n";
  source += "  {\n";

  for (auto argument: function->GetArguments())
  {
    source += "    " + GenerateAddAttribute(argument) + ";\n";
  }

  source += "  }\n";


  source += GenerateFunctionVoidInvokeMethod(classNode, function, false);
  source += GenerateFunctionVoidInvokeMethod(classNode, function, true);
  source += GenerateFunctionValueInvokeMethod(classNode, function, false);
  source += GenerateFunctionValueInvokeMethod(classNode, function, true);
  source += GenerateFunctionReferenceInvokeMethod(classNode, function, false, false);
  source += GenerateFunctionReferenceInvokeMethod(classNode, function, false, true);
  source += GenerateFunctionReferenceInvokeMethod(classNode, function, true, false);
  source += GenerateFunctionReferenceInvokeMethod(classNode, function, true, true);
  source += GenerateFunctionPointerInvokeMethod(classNode, function, false, false);
  source += GenerateFunctionPointerInvokeMethod(classNode, function, false, true);
  source += GenerateFunctionPointerInvokeMethod(classNode, function, true, false);
  source += GenerateFunctionPointerInvokeMethod(classNode, function, true, true);


  source += "";
  source += "};\n\n\n";

  for (size_t i = 0, in = nss.size(); i < in; i++)
  {
    source += "}\n\n";
  }

  return source;
}

std::string
ClassGenerator::GenerateFunctionVoidInvokeMethod(ClassNode *classNode, FunctionNode *function, bool constMethod)
{
  std::string fkt;
  std::string sConst = constMethod ? "const " : "";

  fkt += "\n";
  fkt += "  virtual void InvokeVoidImpl (" + sConst + "ce::iObject* obj, ...) const\n";
  fkt += "  {\n";
  if (constMethod && !function->IsConst())
  {
    fkt += "    throw ce::BadMethodInvokation(\"[" + function->GetName() + "] No const-call\");\n";
  }
  else
  {
    fkt +=
        "    " + sConst + classNode->GetName() + " *d = ce::QueryClass<" + sConst + classNode->GetName() + ">(obj);\n";
    fkt += "    if (!d)\n";
    fkt += "    {\n";
    fkt += "      throw ce::BadMethodInvokation(\"[" + function->GetName() + "] Object is not instance of " +
           classNode->GetName() + "\");\n";
    fkt += "    }\n\n";

    fkt += "    va_list lst;\n";
    fkt += "    va_start(lst, obj);\n";

    const std::vector<Argument> &arguments = function->GetArguments();
    for (auto                   argument: arguments)
    {
      std::string type = GenerateTypeForMethodInvokation(argument.GetType());
      fkt += "    " + type + argument.GetName() + " = va_arg(lst, " + type + ");\n";

    }
    fkt += "    va_end(lst);\n";

    fkt += "    d->" + function->GetName() + "(";

    for (size_t i = 0, in = arguments.size(); i < in; ++i)
    {
      const Argument &argument = arguments[i];
      fkt += "*" + argument.GetName();
      if (i + 1 < in)
      {
        fkt += ", ";
      }
    }
    fkt += ");\n";
  }

  fkt += "  }\n";
  return fkt;
}

std::string
ClassGenerator::GenerateFunctionValueInvokeMethod(ClassNode *classNode, FunctionNode *function, bool constMethod)
{
  std::string fkt;
  std::string sConst = constMethod ? "const " : "";

  fkt += "\n";
  fkt += "  virtual void InvokeValueImpl (" + sConst + "ce::iObject* obj, ...) const\n";
  fkt += "  {\n";
  if (!function->GetReturnValue().IsValue() || function->GetReturnValue().IsVoid())
  {
    fkt += "    throw ce::BadMethodInvokation(\"[" + function->GetName() + "] No value-call\");\n";
  }
  else if (constMethod && !function->IsConst())
  {
    fkt += "    throw ce::BadMethodInvokation(\"[" + function->GetName() + "] No const-call\");\n";
  }
  else
  {
    fkt +=
        "    " + sConst + classNode->GetName() + " *d = ce::QueryClass<" + sConst + classNode->GetName() + ">(obj);\n";
    fkt += "    if (!d)\n";
    fkt += "    {\n";
    fkt += "      throw ce::BadMethodInvokation(\"[" + function->GetName() + "] Object is not instance of " +
           classNode->GetName() + "\");\n";
    fkt += "    }\n\n";

    fkt += "    va_list lst;\n";
    fkt += "    va_start(lst, obj);\n";

    const std::vector<Argument> &arguments = function->GetArguments();
    for (auto                   argument: arguments)
    {
      std::string type = GenerateTypeForMethodInvokation(argument.GetType());
      fkt += "    " + type + argument.GetName() + " = va_arg(lst, " + type + ");\n";

    }
    std::string type = GenerateTypeForMethodReturnInvokation(function->GetReturnValue());
    fkt += "    " + type + "____ptr____result____ = va_arg(lst, " + type + ");\n";
    fkt += "    va_end(lst);\n";

    fkt += "    *____ptr____result____ = d->" + function->GetName() + "(";


    for (size_t i = 0, in = arguments.size(); i < in; ++i)
    {
      const Argument &argument = arguments[i];
      fkt += "*" + argument.GetName();
      if (i + 1 < in)
      {
        fkt += ", ";
      }
    }
    fkt += ");\n";
  }

  fkt += "  }\n";
  return fkt;
}


std::string ClassGenerator::GenerateFunctionReferenceInvokeMethod(ClassNode *classNode,
                                                                  FunctionNode *function,
                                                                  bool constReturn,
                                                                  bool constMethod)
{
  std::string fkt;
  std::string sConst = constMethod ? "const " : "";

  std::string rConst = constReturn ? "const " : "";
  std::string mConst = constReturn ? "Const" : "";

  fkt += "\n";
  fkt +=
      "  virtual " + rConst + "void *Invoke" + mConst + "ReferenceImpl (" + sConst + "ce::iObject* obj, ...) const\n";
  fkt += "  {\n";
  if (!function->GetReturnValue().IsReference())
  {
    fkt += "    throw ce::BadMethodInvokation(\"[" + function->GetName() + "] No reference-call\");\n";
  }
  else if (!constReturn && function->GetReturnValue().IsConst())
  {
    fkt += "    throw ce::BadMethodInvokation(\"[" + function->GetName() + "] Return value is non-const\");\n";
  }
  else if (constMethod && !function->IsConst())
  {
    fkt += "    throw ce::BadMethodInvokation(\"[" + function->GetName() + "] No const-call\");\n";
  }
  else
  {
    fkt +=
        "    " + sConst + classNode->GetName() + " *d = ce::QueryClass<" + sConst + classNode->GetName() + ">(obj);\n";
    fkt += "    if (!d)\n";
    fkt += "    {\n";
    fkt += "      throw ce::BadMethodInvokation(\"[" + function->GetName() + "] Object is not instance of " +
           classNode->GetName() + "\");\n";
    fkt += "    }\n\n";

    fkt += "    va_list lst;\n";
    fkt += "    va_start(lst, obj);\n";

    const std::vector<Argument> &arguments = function->GetArguments();
    for (auto                   argument: arguments)
    {
      std::string type = GenerateTypeForMethodInvokation(argument.GetType());
      fkt += "    " + type + argument.GetName() + " = va_arg(lst, " + type + ");\n";

    }
    fkt += "    va_end(lst);\n";

    fkt += "    return reinterpret_cast<" + rConst + "void*>(&d->" + function->GetName() + "(";

    for (size_t i = 0, in = arguments.size(); i < in; ++i)
    {
      const Argument &argument = arguments[i];
      fkt += "*" + argument.GetName();
      if (i + 1 < in)
      {
        fkt += ", ";
      }
    }
    fkt += "));\n";
  }

  fkt += "  }\n";
  return fkt;
}


std::string ClassGenerator::GenerateFunctionPointerInvokeMethod(ClassNode *classNode,
                                                                FunctionNode *function,
                                                                bool constReturn,
                                                                bool constMethod)
{
  std::string fkt;
  std::string sConst = constMethod ? "const " : "";

  std::string rConst = constReturn ? "const " : "";
  std::string mConst = constReturn ? "Const" : "";

  fkt += "\n";
  fkt += "  virtual " + rConst + "void *Invoke" + mConst + "PointerImpl (" + sConst + "ce::iObject* obj, ...) const\n";
  fkt += "  {\n";
  if (!function->GetReturnValue().IsPointer())
  {
    fkt += "    throw ce::BadMethodInvokation(\"[" + function->GetName() + "] No pointer-call\");\n";
  }
  else if (!constReturn && function->GetReturnValue().IsConst())
  {
    fkt += "    throw ce::BadMethodInvokation(\"[" + function->GetName() + "] Return value is non-const\");\n";
  }
  else if (constMethod && !function->IsConst())
  {
    fkt += "    throw ce::BadMethodInvokation(\"[" + function->GetName() + "] No const-call\");\n";
  }
  else
  {
    fkt +=
        "    " + sConst + classNode->GetName() + " *d = ce::QueryClass<" + sConst + classNode->GetName() + ">(obj);\n";
    fkt += "    if (!d)\n";
    fkt += "    {\n";
    fkt += "      throw ce::BadMethodInvokation(\"Object is not instance of " + classNode->GetName() + "\");\n";
    fkt += "    }\n\n";

    fkt += "    va_list lst;\n";
    fkt += "    va_start(lst, obj);\n";

    const std::vector<Argument> &arguments = function->GetArguments();
    for (auto                   argument: arguments)
    {
      std::string type = GenerateTypeForMethodInvokation(argument.GetType());
      fkt += "    " + type + argument.GetName() + " = va_arg(lst, " + type + ");\n";

    }
    fkt += "    va_end(lst);\n";

    fkt += "    return reinterpret_cast<" + rConst + "void*>(d->" + function->GetName() + "(";

    for (size_t i = 0, in = arguments.size(); i < in; ++i)
    {
      const Argument &argument = arguments[i];
      fkt += "*" + argument.GetName();
      if (i + 1 < in)
      {
        fkt += ", ";
      }
    }
    fkt += "));\n";
  }

  fkt += "  }\n";
  return fkt;
}


std::string ClassGenerator::GenerateTypeForMethodInvokation(const TypeDef &typeDef)
{

  std::string type;
  if (typeDef.IsConst())
  {
    type = "const ";
  }
  type += typeDef.GetTypeName() + " ";
  if (typeDef.IsValue() || typeDef.IsReference())
  {
    type += "*";
  }
  else if (typeDef.IsPointer())
  {
    type += "**";
  }
  else if (typeDef.IsPointerToPointer())
  {
    type += "***";
  }

  return type;
}

std::string ClassGenerator::GenerateTypeForMethodReturnInvokation(const TypeDef &typeDef)
{

  std::string type;
  type += typeDef.GetTypeName() + " ";
  if (typeDef.IsValue() || typeDef.IsReference())
  {
    type += "*";
  }
  else if (typeDef.IsPointer())
  {
    type += "**";
  }
  else if (typeDef.IsPointerToPointer())
  {
    type += "***";
  }
  return type;
}

bool startsWith(const std::string &str, const std::string &part)
{
  return str.length() >= part.length() && str.substr(0, part.length()) == part;
}


std::string ClassGenerator::GenerateAttribute(ClassNode *classNode,
                                              std::list<NamespaceNode *> &nss,
                                              MemberNode *member,
                                              const std::string &visibility,
                                              CSMetaNode *meta)
{
  std::string propName = member->GetName();
  if (startsWith(propName, "m_"))
  {
    propName = propName.substr(2);
  }
  else if (startsWith(propName, "_"))
  {
    propName = propName.substr(1);
  }
  if (propName[0] >= 'a' && propName[0] <= 'z')
  {
    propName[0] += 'A' - 'a';
  }
  std::string propClassName = classNode->GetName() + "Class_Property_" + propName;


  TypeDef type = member->GetType();
  TypeDef containerType;
  if (!type.GetSubTypes().empty())
  {
    containerType = type;
    if (type.GetSubTypes().size() > 1)
    {
      return "";
    }
    type = type.GetSubTypes()[0];
  }

  m_propertyClasses.push_back(propClassName);

  std::string prop;
  for (auto   ns: nss)
  {
    prop += "namespace " + ns->GetName() + "\n";
    prop += "{\n";
  }
  prop += "\nclass " + propClassName + " : public ce::Property\n";
  prop += "{\n";
  prop += "public:\n";
  prop += "  " + propClassName + "()\n";
  prop += "    : ce::Property(" + GenerateCSValueDeclaration(containerType, false) + ", \"" + propName + "\", " +
          GenerateCSValueDeclaration(type, true) + ")\n";
  prop += "  {\n";
  prop += "  }\n\n";


  std::string sType      = GenerateTypeForAttribute(type);
  std::string sInputType = GenerateInputTypeForAttribute(type);
  prop += "  virtual void SetValue (ce::iObject *object, void *data) const\n";
  prop += "  {\n";
  if (visibility == std::string("public") && !type.IsConst())
  {
    prop += "    " + classNode->GetName() + " *d = ce::QueryClass<" + classNode->GetName() + ">(object);\n";
    prop += "    if (!d)\n";
    prop += "    {\n";
    prop += "      throw ce::BadMethodInvokation(\"Object is not instance of " + classNode->GetName() + "\");\n";
    prop += "    }\n\n";

    if (!type.IsPointer() || meta->Has("Native"))
    {
      prop +=
          "    d->" + member->GetName() + " = *reinterpret_cast<" + GenerateInputTypeForAttribute(type) + ">(data);\n";
    }
    else
    {
      prop += "    " + type.GetText() + " value = *reinterpret_cast<" + type.GetText() + "*>(data);\n";
      prop += "    CS_SET(d->" + member->GetName() + ", value);\n";
    }

  }
  else
  {
    prop += "    throw ce::BadMethodInvokation(\"Property [" + classNode->GetName() + "::" + member->GetName() +
            "] is not public\");\n";
  }
  prop += "  }\n\n";

  prop += "  virtual const void* GetValue (const ce::iObject *object) const\n";
  prop += "  {\n";
  if (visibility == std::string("public"))
  {
    prop += "    const " + classNode->GetName() + " *d = ce::QueryClass<const " + classNode->GetName() + ">(object);\n";
    prop += "    if (!d)\n";
    prop += "    {\n";
    prop += "      throw ce::BadMethodInvokation(\"Object is not instance of " + classNode->GetName() + "\");\n";
    prop += "    }\n\n";
    prop += "    return reinterpret_cast<const void*>(&d->" + member->GetName() + ");";
  }
  else
  {
    prop += "    throw ce::BadMethodInvokation(\"Property [" + classNode->GetName() + "::" + member->GetName() +
            "] is not public\");\n";
  }
  prop += "  }\n\n";


  prop += "  virtual void* GetValue (ce::iObject *object) const\n";
  prop += "  {\n";
  if (visibility == std::string("public"))
  {
    prop += "    " + classNode->GetName() + " *d = ce::QueryClass<" + classNode->GetName() + ">(object);\n";
    prop += "    if (!d)\n";
    prop += "    {\n";
    prop += "      throw ce::BadMethodInvokation(\"Object is not instance of " + classNode->GetName() + "\");\n";
    prop += "    }\n\n";
    prop += "    return reinterpret_cast<void*>(&d->" + member->GetName() + ");";
  }
  else
  {
    prop += "    throw ce::BadMethodInvokation(\"Property [" + classNode->GetName() + "::" + member->GetName() +
            "] is not public\");\n";
  }
  prop += "  }\n\n";

  prop += "};\n\n";

  for (size_t i = 0, in = nss.size(); i < in; i++)
  {
    prop += "}\n";
  }

  return prop;
}

std::string ClassGenerator::GenerateTypeForAttribute(const TypeDef &typeDef)
{
  std::string type;
  if (typeDef.IsConst())
  {
    type += "const ";
  }
  type += typeDef.GetTypeName();
  if (typeDef.IsValue() || typeDef.IsReference())
  {
    type += "&";
  }
  else if (typeDef.IsPointer())
  {
    type += "*";
  }
  else if (typeDef.IsPointerToPointer())
  {
    type += "**";
  }
  return type;
}


std::string ClassGenerator::GenerateInputTypeForAttribute(const TypeDef &typeDef)
{
  std::string type;
  if (typeDef.IsConst())
  {
    type += "const ";
  }
  type += typeDef.GetTypeName();
  if (typeDef.IsValue() || typeDef.IsReference())
  {
    type += "*";
  }
  else if (typeDef.IsPointer())
  {
    type += "**";
  }
  else if (typeDef.IsPointerToPointer())
  {
    type += "***";
  }
  return type;
}

std::string ClassGenerator::GenerateCSValueDeclaration(const TypeDef &typeDef, bool withSubTypes)
{
  std::string evmm = "ce::eVMM_Value";
  if (typeDef.IsReference())
  {
    evmm = "ce::eVMM_Reference";
  }
  else if (typeDef.IsPointer())
  {
    evmm = "ce::eVMM_Pointer";
  }
  else if (typeDef.IsPointerToPointer())
  {
    evmm = "ce::eVMM_PointerToPointer";
  }


  return std::string("ce::ValueDeclaration(") +
         (typeDef.IsConst() ? "ce::eC_Const" : "ce::eC_NonConst") + ", " +
         "\"" + typeDef.GetTypeName(withSubTypes) + "\", " +
         evmm + ")";
}

std::string ClassGenerator::GenerateAddAttribute(const Argument &argument)
{
  return std::string("AddAttribute(ce::FunctionAttribute (") +
         GenerateCSValueDeclaration(argument.GetType()) + ", " +
         "\"" + argument.GetName() + "\"))";

}


std::string ClassGenerator::GenerateClass(ClassNode *classNode, std::list<NamespaceNode *> &nss, CSMetaNode *meta)
{
  std::string cls;
  std::string fns = Generator::GetFullNamespaceName(nss);

  std::string className      = classNode->GetName();
  std::string classClassName = classNode->GetName() + "Class";

  //
  // Singleton getter
  cls += fns + classClassName + " *" + fns + classClassName + "::Get()\n";
  cls += "{\n";
  cls += "  static " + fns + classClassName + " static_class;\n";
  cls += "  return &static_class;\n";
  cls += "};\n\n";

  // 
  // Class constructor
  cls += fns + classClassName + "::" + classClassName + "()\n";
  cls += "  : ce::Class(\"" + fns + className + "\")\n";
  cls += "{\n";
  for (auto super: classNode->GetSupers())
  {
    if (super.IsCSSuper())
    {
      cls += "  AddSuperClass(" + super.GetType().GetTypeName() + "::GetStaticClass());\n";
    }
  }

  for (auto propertyName: m_propertyClasses)
  {
    cls += "  AddProperty(new " + fns + propertyName + "());\n";
  }

  for (auto functionName: m_functionClasses)
  {
    cls += "  AddFunction(new " + fns + functionName + "());\n";
  }

  cls += "}\n\n";

  cls += "ce::iObject *" + fns + classClassName + "::CreateInstance() const\n";
  cls += "{\n";
  if (!classNode->HasPureVirtualMethod() && classNode->HasPublicDefaultConstructor() && !meta->Has("Virtual"))
  {
    cls += "  return static_cast<ce::iObject*>(new " + fns + className + "());\n";
  }
  else
  {
    cls += "  throw ce::InstanciationException(\"" + fns + className + "\");\n";
  }
  cls += "}\n\n";


  cls += "const ce::Class *" + fns + className + "::GetClass() const\n";
  cls += "{\n";
  cls += "  return " + fns + classClassName + "::Get();\n";
  cls += "}\n\n";
  cls += "const ce::Class *" + fns + className + "::GetStaticClass()\n";
  cls += "{\n";
  cls += "  return " + fns + classClassName + "::Get();\n";
  cls += "}\n\n";
  cls += GenerateQueryClass(classNode, nss, meta, false);
  cls += GenerateQueryClass(classNode, nss, meta, true);


  cls += GenerateCreateJObject(classNode, nss, meta);

  return cls;

}


std::string
ClassGenerator::GenerateQueryClass(ClassNode *classNode, std::list<NamespaceNode *> &nss, CSMetaNode *, bool _const)
{
  std::string fns = Generator::GetFullNamespaceName(nss);

  std::string query          = "";
  std::string sConst         = _const ? "const " : "";
  std::string className      = classNode->GetName();
  std::string classClassName = classNode->GetName() + "Class";

  query += sConst + "void *" + fns + className + "::QueryClass(const ce::Class* clazz) " + sConst + "\n";
  query += "{\n";
  query += "  if (clazz == " + fns + classClassName + "::Get())\n";
  query += "  {\n";
  query += "    return static_cast<" + sConst + fns + className + "*>(this);\n";
  query += "  }\n";
  query += "  " + sConst + "void *super = nullptr;\n";

  for (auto super: classNode->GetSupers())
  {
    std::string superType = super.GetType().GetTypeName();
    if (!(super.IsCSSuper()
          || superType == "iObject"
          || superType == "ce::iObject"
          || superType == "Object"
          || superType == "ce::Object"
    ))
    {
      continue;
    }

    query += "  super = " + super.GetType().GetTypeName() + "::QueryClass(clazz);\n";
    query += "  if (super)\n";
    query += "  {\n";
    query += "    return super;\n";
    query += "  }\n";

  }

  query += "  return nullptr;\n";
  query += "}\n\n";
  return query;
}

std::string convert_java_class_path(const std::string &jclass)
{
  std::string classPath = jclass;
  if (!classPath.empty() && classPath[0] == '"')
  {
    classPath = classPath.substr(1);
  }
  if (!classPath.empty() && classPath[classPath.length() - 1] == '"')
  {
    classPath = classPath.substr(0, classPath.length() - 1);
  }
  while (true)
  {
    unsigned __int64 idx = classPath.find(".");
    if (idx == std::string::npos)
    {
      break;
    }
    classPath.replace(idx, 1, "/");
  }
  return classPath;
}

std::string ClassGenerator::GenerateCreateJObject(ce::moc::ClassNode *classNode,
                                                  std::list<NamespaceNode *> &nss,
                                                  ce::moc::CSMetaNode *meta)
{
  std::string fns       = Generator::GetFullNamespaceName(nss);
  std::string className = classNode->GetName();
  std::string getter;
  getter += "#ifdef CS_JAVA\n";
  getter += "jobject " + fns + className + "::CreateJObject() const\n";
  getter += "{\n";

  if (meta->Has("jclass"))
  {
    std::string jclass = meta->Get("jclass");
    jclass = convert_java_class_path(jclass);
    getter += "  static jclass cls = ce::Java::Get() ? ce::Java::Get()->FindClass (\"" + jclass + "\") : nullptr;\n";
    getter += "  if (cls)\n";
    getter += "  {\n";
    getter += "    static jmethodID ctor = ce::Java::Get()->GetMethodID(cls, \"<init>\", \"(J)V\");\n";
    getter += "    if (ctor)\n";
    getter += "    {\n";
    getter += "      jobject obj = ce::Java::Get()->NewObject(cls, ctor, reinterpret_cast<jlong>(this));\n";
    getter += "      if (obj)\n";
    getter += "      {\n";
    getter += "        obj = ce::Java::Get()->NewGlobalRef(obj);\n";
    getter += "        return obj;\n";
    getter += "      }\n";
    getter += "    }\n";
    getter += "  }\n";
  }
  else
  {
    for (auto super: classNode->GetSupers())
    {
      std::string superType = super.GetType().GetTypeName();
      if (super.IsCSSuper()
          && superType != "iObject"
          && superType != "ce::iObject"
          && superType != "Object"
          && superType != "ce::Object"
          )
      {
        getter += "  {\n";
        getter += "    jobject obj = " + superType + "::CreateJObject();\n";
        getter += "    if (obj)\n";
        getter += "    {\n";
        getter += "      return obj;\n";
        getter += "    }\n";
        getter += "  }\n";
      }
    }

  }
  getter += " return nullptr;\n";

  getter += "}\n";
  getter += "#endif\n";


  return getter;
}


}
