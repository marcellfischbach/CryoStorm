
#pragma once

#include <list>
#include <string>
#include <vector>

namespace cs::moc
{




class ASTNode;
class ClassNode;
class NamespaceNode;

class Generator
{
public:
  Generator();

  void SetRoot(ASTNode* node);

  static std::vector<ClassNode*> FindAllMajorClasses(ASTNode* node);
  std::vector<ClassNode*> FindAllMajorClasses();
  static std::list<NamespaceNode*> GetAllNamespaces(ASTNode* node);
  static std::string GetFullNamespaceName(std::list<NamespaceNode*>& namespaceNodes);
  static std::string GetEscapedNamespaceName(std::list<NamespaceNode*>& namespaceNodes);

private:
  static void ScanClasses(ASTNode* node, std::vector<ClassNode*>& classes);

  
  ASTNode* m_root;
};


}
