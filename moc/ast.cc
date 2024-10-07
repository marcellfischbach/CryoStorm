

#include <ast.hh>
#include <algorithm>


namespace cs::moc
{

ASTNode::ASTNode(ASTNodeType type)
  : m_type(type)
  , m_parent(nullptr)
{

}

ASTNodeType ASTNode::GetType() const
{
  return m_type;
}

ASTNode* ASTNode::FindChildNode(ASTNodeType type)
{
  for (auto child : m_children)
  {
    if (child->GetType() == type)
    {
      return child;
    }
  }
  return nullptr;
}


ASTNode* ASTNode::FindParentNode(ASTNodeType type)
{
  ASTNode* testNode = this;
  while (testNode)
  {
    if (testNode->GetType() == type)
    {
      return testNode;
    }
    testNode = testNode->GetParent();
  }
  return nullptr;
}

ASTNode* ASTNode::FindPrevSibling(ASTNodeType type)
{
  if (!m_parent)
  {
    return nullptr;
  }

  std::vector<ASTNode*>::iterator it = std::ranges::find(m_parent->m_children.begin(), m_parent->m_children.end(), this);
  if (it != m_parent->m_children.begin())
  {
    --it;
    for (;; it--)
    {
      if ((*it)->GetType() == type)
      {
        return *it;
      }
      if (it == m_parent->m_children.begin())
      {
        break;
      }
    }
  }


  return nullptr;
}

ASTNode* ASTNode::FindNextSibling(ASTNodeType type)
{
  if (!m_parent)
  {
    return nullptr;
  }

  std::vector<ASTNode*>::iterator it = std::ranges::find(m_parent->m_children.begin(), m_parent->m_children.end(), this);
  if (it != m_parent->m_children.end())
  {
    for (++it; it != m_parent->m_children.end(); it++)
    {
      if ((*it)->GetType() == type)
      {
        return *it;
      }
    }
  }


  return nullptr;

}


ASTNode* ASTNode::FindPrevSibling()
{
  if (!m_parent)
  {
    return nullptr;
  }

  std::vector<ASTNode*>::iterator it = std::ranges::find(m_parent->m_children.begin(), m_parent->m_children.end(), this);
  if (it == m_parent->m_children.end() || it == m_parent->m_children.begin())
  {
    return nullptr;
  }

  it--;
  return *it;
}

ASTNode* ASTNode::FindNextSibling()
{
  if (!m_parent)
  {
    return nullptr;
  }

  std::vector<ASTNode*>::iterator it = std::ranges::find(m_parent->m_children.begin(), m_parent->m_children.end(), this);
  if (it == m_parent->m_children.end())
  {
    return nullptr;
  }

  it++;
  if (it == m_parent->m_children.end())
  {
    return nullptr;
  }

  return *it;

}


void ASTNode::Add(ASTNode* node)
{
  if (node)
  {
    m_children.push_back(node);
    node->m_parent = this;
  }
}

ASTNode* ASTNode::GetParent()
{
  return m_parent;
}

const ASTNode* ASTNode::GetParent() const
{
  return m_parent;
}

const std::vector<ASTNode*>& ASTNode::GetChildren() const
{
  return m_children;
}

void ASTNode::DebugNode(int ind)
{
  for (int i = 0; i < ind; ++i)
  {
    printf("  ");
  }

  Debug();
  printf("\n");

  for (auto child : m_children)
  {
    child->DebugNode(ind + 1);
  }
}

BlockNode::BlockNode()
  : ASTNode(eANT_Block)
{

}

void BlockNode::Debug()
{
  printf("BLCK");
}

NamespaceNode::NamespaceNode()
  : ASTNode(eANT_Namespace)
{

}

void NamespaceNode::SetName(const std::string & name)
{
  m_name = name;
}

const std::string& NamespaceNode::GetName() const
{
  return m_name;
}

void NamespaceNode::SetAlias(const std::string & alias)
{
  m_alias = alias;
}

const std::string& NamespaceNode::GetAlias() const
{
  return m_alias;
}

void NamespaceNode::Debug()
{
  if (m_alias.empty())
  {
    printf("NS[%s]", m_name.c_str());
  }
  else
  {
    printf("NS[%s=%s]", m_alias.c_str(), m_name.c_str());
  }
}






ClassSuperDefinition::ClassSuperDefinition(bool csSuper, const TypeDef & type, const std::string & visibility, bool virtuality)
  : m_csSuper(csSuper)
  , m_type(type)
  , m_visibility(visibility)
  , m_virtual(virtuality)
{

}

bool ClassSuperDefinition::IsCSSuper() const
{
  return m_csSuper;
}

const TypeDef& ClassSuperDefinition::GetType() const
{
  return m_type;
}

const std::string& ClassSuperDefinition::GetVisibility() const
{
  return m_visibility;
}

bool ClassSuperDefinition::IsVirtual() const
{
  return m_virtual;
}

CSMetaNode::CSMetaNode(CSMetaNode::MetaType type)
  : ASTNode(eANT_CSMeta)
  , m_type(type)
{

}

void CSMetaNode::Add(const CSMetaNode::Attribute & attribute)
{
  m_attributes.push_back(attribute);
}

bool CSMetaNode::Has(const std::string& key) const
{
  for (auto attribute : m_attributes)
  {
    if (attribute.key == key)
    {
      return true;
    }
  }
  return false;
}

std::string CSMetaNode::Get(const std::string& key) const
{
  for (auto attribute : m_attributes)
  {
    if (attribute.key == key)
    {
      return attribute.value;
    }
  }
  return "";
}

void CSMetaNode::Debug()
{
  switch (m_type)
  {
  case eMT_Class:
    printf("CS_CLASS(");
    break;
  case eMT_Property:
    printf("CS_PROPERTY(");
    break;
  case eMT_Function:
    printf("CS_FUNCTION(");
    break;
  }
  for (auto attribute : m_attributes)
  {
    printf("[");
    if (!attribute.key.empty())
    {
      printf("%s=", attribute.key.c_str());
    }
    printf("%s]", attribute.value.c_str());
  }
  printf(")");
}

ClassNode::ClassNode()
  : ASTNode(eANT_Class)
  , m_struct(false)
{

}

void ClassNode::SetStruct(bool strct)
{
  m_struct = strct;
}

bool ClassNode::IsStruct() const
{
  return m_struct;
}

void ClassNode::SetName(const std::string & name)
{
  m_name = name;
}

const std::string& ClassNode::GetName() const
{
  return m_name;
}

void ClassNode::AddSuper(const ClassSuperDefinition & super)
{
  m_supers.push_back(super);
}

const std::vector<ClassSuperDefinition>& ClassNode::GetSupers() const
{
  return m_supers;
}

bool ClassNode::HasPureVirtualMethod()
{
  BlockNode* blockNode = static_cast<BlockNode*>(FindChildNode(eANT_Block));
  if (!blockNode)
  {
    return false;
  }

  FunctionNode* functionNode = static_cast<FunctionNode*>(blockNode->FindChildNode(eANT_Function));
  while (functionNode)
  {
    if (functionNode->IsVirtual() && functionNode->IsPureVirtual())
    {
      return true;
    }
    functionNode = static_cast<FunctionNode*>(functionNode->FindNextSibling(eANT_Function));
  }
  return false;
}

bool ClassNode::HasPublicDefaultConstructor()
{
  BlockNode* blockNode = static_cast<BlockNode*>(FindChildNode(eANT_Block));
  if (!blockNode)
  {
    return false;
  }

  bool hasConstructor = false;
  FunctionNode* functionNode = static_cast<FunctionNode*>(blockNode->FindChildNode(eANT_Function));
  while (functionNode)
  {
    if (!functionNode->GetReturnValue().IsValid() &&
      functionNode->GetName() == GetName())
    {

      hasConstructor = true;

      VisibilityNode* visNode = static_cast<VisibilityNode*>(functionNode->FindPrevSibling(eANT_Visibility));
      if (visNode == nullptr && m_struct || visNode != nullptr &&  visNode->GetVisibility() == "public")
      {

        // this is a constructor .. but is it a default constructor???
        bool hasParameter = false;
        for (auto argument : functionNode->GetArguments())
        {
          if (!argument.HasDefaultValue())
          {
            hasParameter = true;
            break;
          }
        }
        if (!hasParameter)
        {
          return true;
        }
      }
    }
    functionNode = static_cast<FunctionNode*>(functionNode->FindNextSibling(eANT_Function));
  }

  return !hasConstructor;

}

void ClassNode::Debug()
{
  printf("%s[%s", m_struct ? "STRCT" : "CLS", m_name.c_str());
  for (auto super : m_supers)
  {
    if (super.IsCSSuper())
    {
      printf(" CS_SUPER(%s%s%s)",
        super.IsVirtual() ? "virtual " : "",
        super.GetVisibility().empty() ? "" : (super.GetVisibility() + " ").c_str(),
        super.GetType().GetText().c_str());
    }
    else
    {

      printf(" native(%s%s%s)",
        super.IsVirtual() ? "virtual " : "",
        super.GetVisibility().empty() ? "" : (super.GetVisibility() + " ").c_str(),
        super.GetType().GetText().c_str()
      );
    }
  }
  printf("]");
}


VisibilityNode::VisibilityNode(const std::string & visibility)
  : ASTNode(eANT_Visibility)
  , m_visibility(visibility)
{


}

const std::string& VisibilityNode::GetVisibility() const
{
  return m_visibility;
}

void VisibilityNode::Debug()
{
  printf("VSBLT[%s]", m_visibility.c_str());
}


Argument::Argument(const TypeDef & type, const std::string & name, bool defaultValue)
  : m_type(type)
  , m_name(name)
  , m_defaultValue(defaultValue)
{

}

const TypeDef& Argument::GetType() const
{
  return m_type;
}

const std::string& Argument::GetName() const
{
  return m_name;
}

bool Argument::HasDefaultValue() const
{
  return m_defaultValue;
}

std::string Argument::GetText() const
{
  std::string text = m_type.GetText();
  if (!m_name.empty())
  {
    text += " " + m_name;
  }
  return text;
}

FunctionNode::FunctionNode()
  : ASTNode(eANT_Function)
  , m_name("")
  , m_virtual(false)
  , m_pureVirtual(false)
  , m_const(false)
{

}

void FunctionNode::SetName(const std::string & name)
{
  m_name = name;
}

const std::string& FunctionNode::GetName() const
{
  return m_name;
}

void FunctionNode::SetReturnValue(const TypeDef & returnValue)
{
  m_returnValue = returnValue;
}

const TypeDef& FunctionNode::GetReturnValue() const
{
  return m_returnValue;
}

void FunctionNode::SetVirtual(bool virtuality)
{
  m_virtual = virtuality;
}

bool FunctionNode::IsVirtual() const
{
  return m_virtual;
}

void FunctionNode::SetPureVirtual(bool pureVirtual)
{
  m_pureVirtual = pureVirtual;
}

bool FunctionNode::IsPureVirtual() const
{
  return m_pureVirtual;
}

void FunctionNode::SetConst(bool constness)
{
  m_const = constness;
}

bool FunctionNode::IsConst() const
{
  return m_const;
}

void FunctionNode::Add(const Argument & argument)
{
  m_arguments.push_back(argument);
}

const std::vector<Argument>& FunctionNode::GetArguments() const
{
  return m_arguments;
}

void FunctionNode::Debug()
{
  std::string arguments = "";
  for (size_t i = 0, in = m_arguments.size(); i < in; ++i)
  {
    arguments += m_arguments[i].GetText();
    if (i + 1 < m_arguments.size())
    {
      arguments += ", ";
    }
  }

  printf("FNCTN[%s%s%s%s(%s)]: %s",
    m_virtual ? "VIRTUAL " : "",
    m_name.c_str(),
    m_const ? " CONST" : "",
    m_pureVirtual ? " PURE " : "",
    arguments.c_str(),
    m_returnValue.GetText().c_str());


}


MemberNode::MemberNode()
  : ASTNode(eANT_Member)
{

}


void MemberNode::SetName(const std::string & name)
{
  m_name = name;
}

const std::string& MemberNode::GetName() const
{
  return m_name;
}

void MemberNode::SetType(const TypeDef & type)
{
  m_type = type;
}

const TypeDef& MemberNode::GetType() const
{
  return m_type;
}

void MemberNode::Debug()
{
  printf("MMBR [%s:%s]",
    m_name.c_str(), m_type.GetText().c_str());
}



TypeDef::TypeDef()
  : m_const(false)
  , m_constPtr(false)
{

}

void TypeDef::Add(const Token & token)
{
  switch (token.GetType())
  {
  case eTT_Const:
    if (m_mems.empty())
    {
      m_const = true;
    }
    else
    {
      m_constPtr = true;
    }
    break;
  case eTT_Ampersand:
  case eTT_Asterisk:
  case eTT_DoubleAsterisk:
    m_mems.push_back(token);
    break;
  default:
    m_tokens.push_back(token);
    break;
  }
}

void TypeDef::Add(const TypeDef & subType)
{
  m_subTypes.push_back(subType);
}

void TypeDef::AddFront(const Token & token)
{
  switch (token.GetType())
  {
  case eTT_Const:
    m_const = true;
    break;
  case eTT_Ampersand:
  case eTT_Asterisk:
  case eTT_DoubleAsterisk:
    m_constPtr = m_constPtr || m_const;
    m_const = false;
    m_mems.insert(m_mems.begin(), token);
    break;
  default:
    m_tokens.insert(m_tokens.begin(), token);
  }
}

void TypeDef::AddFront(const TypeDef & subType)
{
  m_subTypes.insert(m_subTypes.begin(), subType);
}
void TypeDef::SetConst(bool constness)
{
  m_const = constness;
}

bool TypeDef::IsConst() const
{
  return m_const;
}

bool TypeDef::IsValue() const
{
  return this->m_mems.size() == 0;
}

bool TypeDef::IsReference() const
{
  for (auto mem : m_mems)
  {
    if (mem.GetType() == eTT_Ampersand)
    {
      return true;
    }
  }
  return false;
}

bool TypeDef::IsPointer() const
{
  for (auto mem : m_mems)
  {
    if (mem.GetType() == eTT_Asterisk)
    {
      return true;
    }
  }
  return false;
}

bool TypeDef::IsPointerToPointer() const
{
  for (auto mem : m_mems)
  {
    if (mem.GetType() == eTT_DoubleAsterisk)
    {
      return true;
    }
  }
  return false;
}

bool TypeDef::IsVoid() const
{
  return GetTypeName() == "void";
}

bool TypeDef::IsValid() const
{
  return !m_tokens.empty();
}

const std::vector<TypeDef>& TypeDef::GetSubTypes() const
{
  return m_subTypes;
}

std::string TypeDef::GetTypeName(bool withSubTypes) const
{
  std::string typeName;
  for (auto tkn : m_tokens)
  {
    typeName += tkn.Get();
  }
  if (!m_subTypes.empty() && withSubTypes)
  {
    typeName += "<";
    for (size_t i = 0, in = m_subTypes.size(); i < in; ++i)
    {
      typeName += m_subTypes[i].GetText();
      if (i + 1 < in)
      {
        typeName += ", ";
      }
    }
    typeName += ">";
  }
  return typeName;
}



std::string TypeDef::GetText() const
{
  std::string text;
  if (m_const)
  {
    text += "const ";
  }
  for (auto tkn : m_tokens)
  {
    text += tkn.Get();
  }
  if (!m_subTypes.empty())
  {
    text += "<";
    for (size_t i = 0, in = m_subTypes.size(); i < in; ++i)
    {
      text += m_subTypes[i].GetText();
      if (i + 1 < in)
      {
        text += ", ";
      }
    }
    text += ">";
  }
  for (auto tkn : m_mems)
  {
    text += tkn.Get();
  }
  if (m_constPtr)
  {
    text += " const";
  }
  return text;
}




TokenNode::TokenNode()
  : ASTNode(eANT_Token)
{

}
TokenNode::TokenNode(const Token & token)
  : ASTNode(eANT_Token)
  , m_token(token)
{

}

void TokenNode::SetToken(const Token & token)
{
  m_token = token;
}

const Token& TokenNode::GetToken() const
{
  return m_token;
}

void TokenNode::Debug()
{
  printf("TKN[%s]", m_token.Get().c_str());
}

}
