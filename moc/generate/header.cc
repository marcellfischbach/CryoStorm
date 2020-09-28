
#include <generate/header.hh>
#include <generate/ioutput.hh>
#include <ast.hh>

namespace Spice::moc
{


HeaderGenerator::HeaderGenerator()
{

}

void HeaderGenerator::Output(const std::string& exp, iOutput* output)
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

    headers += OutputClass(cls, exp);
  }
  if (!classes.empty() && output)
  {
    output->output(headers);
  }
}

std::string HeaderGenerator::OutputClass(ClassNode* clsNode, const std::string& exp)
{
  std::string source;
  ASTNode* parent = clsNode;
  std::list<NamespaceNode*> nss = GetAllNamespaces(clsNode);

  std::string clsName = clsNode->GetName() + "Class";

  source += "#pragma once\n";
  source += "\n";
  source += "#ifdef SPICE_WIN32\n";
  source += "#  ifdef " + exp + "\n";
  source += "#    define " + exp + "_API __declspec(dllexport)\n";
  source += "#  else\n";
  source += "#    define " + exp + "_API __declspec(dllimport)\n";
  source += "#  endif\n";
  source += "#else\n";
  source += "#  define " + exp + "_API\n";
  source += "#endif\n";
  for (auto ns : nss)
  {
    source += "namespace " + ns->GetName() + "\n";
    source += "{\n";
  }

  source += "\nclass " + exp + "_API " + clsName + " : public Spice::Class\n";
  source += "{\n";
  source += "public:\n";
  source += " " + clsName + "();\n";
  source += " static " + clsName  + " *Get();\n";
  source += " virtual Spice::iObject *CreateInstance() const;\n";
  source += "};\n";


  for (auto ns : nss)
  {
    source += "}\n";
  }
  source += "\n";
  return source;
}
}
