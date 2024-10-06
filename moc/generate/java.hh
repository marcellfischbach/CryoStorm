
#pragma once

#include <list>
#include <string>

namespace cryo::moc
{

class ClassNode;
class CSMetaNode;
class FunctionNode;
class NamespaceNode;

class JavaGenerator
{
public:

  std::string OutputClass(ClassNode *classNode,std::list<NamespaceNode *> &nss,  CSMetaNode *classMeta);

private:
  std::string  GenerateFunction(ClassNode *classNode,
                                std::list<NamespaceNode *> &nss,
                                CSMetaNode *clsMeta,
                                FunctionNode *functionNode,
                                CSMetaNode *functionMeta);

};


}