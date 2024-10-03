
#pragma once

#include <list>
#include <string>
#include <vector>

namespace cryo::moc
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

private:
  static void ScanClasses(ASTNode* node, std::vector<ClassNode*>& classes);

  
  ASTNode* m_root;
};


}
