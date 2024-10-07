
#include <generate/header.hh>
#include <generate/ioutput.hh>
#include <ast.hh>

namespace cs::moc
{


HeaderGenerator::HeaderGenerator()
{

}

void HeaderGenerator::Output(iOutput* output)
{
  std::vector<ClassNode*> classes = FindAllMajorClasses();

  std::string headers = "";
  for (auto cls : classes)
  {
    ASTNode* prev = cls->FindPrevSibling();
    if (prev == nullptr || prev->GetType() != eANT_CSMeta)
    {
      continue;
    }

    CSMetaNode* meta = static_cast<CSMetaNode*>(prev);
    if (meta->GetMetaType() != CSMetaNode::eMT_Class)
    {
      continue;
    }

    headers += OutputClass(cls);
  }
  if (!classes.empty() && output)
  {
    output->output(headers);
  }
}

std::string HeaderGenerator::OutputClass(ClassNode* clsNode)
{
  std::string source;
  ASTNode* parent = clsNode;
  std::list<NamespaceNode*> nss = GetAllNamespaces(clsNode);

  std::string clsName = clsNode->GetName() + "Class";

  source += "#pragma once\n";
  source += "\n";
  for (auto ns : nss)
  {
    source += "namespace " + ns->GetName() + "\n";
    source += "{\n";
  }

  source += "\nclass " + clsName + " : public cs::csClass\n";
  source += "{\n";
  source += "public:\n";
  source += " " + clsName + "();\n";
  source += " static " + clsName  + " *Get();\n";
  source += " virtual cs::iObject *CreateInstance() const;\n";
  source += "};\n";


  for (auto ns : nss)
  {
    source += "}\n";
  }
  source += "\n";
  return source;
}
}
