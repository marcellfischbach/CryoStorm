
#include <generate/generator.hh>
#include <ast.hh>



namespace cs::moc
{


Generator::Generator()
  : m_root(nullptr)
{

}

void Generator::SetRoot(ASTNode* root)
{
  m_root = root;
}

std::vector<ClassNode*> Generator::FindAllMajorClasses(ASTNode* root)
{
  std::vector<ClassNode*> classes;

  ScanClasses(root, classes);

  return classes;

}
std::vector<ClassNode*> Generator::FindAllMajorClasses()
{
  return FindAllMajorClasses(m_root);
}

void Generator::ScanClasses(ASTNode* node, std::vector<ClassNode*>& classes)
{
  if (node->GetType() == eANT_Class)
  {

    ASTNode* prev = node->FindPrevSibling();
    if (prev != nullptr && prev->GetType() == eANT_CSMeta)
    {

      CSMetaNode* meta = static_cast<CSMetaNode*>(prev);
      if (meta->GetMetaType() == CSMetaNode::eMT_Class)
      {
        classes.push_back(static_cast<ClassNode*>(node));
      }
    }
  }
  else
  {
    for (auto child : node->GetChildren())
    {
      ScanClasses(child, classes);
    }
  }
}


std::list<NamespaceNode*> Generator::GetAllNamespaces(ASTNode* node)
{
  std::list<NamespaceNode*> ns;
  while (node)
  {
    if (node->GetType() == eANT_Namespace)
    {
      ns.push_front(static_cast<NamespaceNode*>(node));
    }
    node = node->GetParent();
  }
  return ns;
}



std::string Generator::GetFullNamespaceName(std::list<NamespaceNode*>& namespaceNodes)
{
  std::string name;
  for (auto ns : namespaceNodes)
  {
    name += ns->GetName() + "::";
  }

  return name;
}
std::string Generator::GetEscapedNamespaceName(std::list<NamespaceNode*>& namespaceNodes)
{
  std::string name;
  for (auto ns : namespaceNodes)
  {
    name += ns->GetName() + "__";
  }

  return name;
}



}
