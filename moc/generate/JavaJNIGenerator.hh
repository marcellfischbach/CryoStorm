//

#pragma once

#include <string>
#include <list>
#include <generate/JavaSourceGenerator.hh>

namespace cs::moc
{

class ClassNode;
class CSMetaNode;
class FunctionNode;
class NamespaceNode;

class JavaJNIGenerator
{
public:
  std::string OutputClass(ClassNode *classNode,std::list<NamespaceNode *> &nss,  CSMetaNode *classMeta);

private:
  std::string  GenerateFunction(ClassNode *classNode,
                                std::list<NamespaceNode *> &nss,
                                CSMetaNode *clsMeta,
                                FunctionNode *functionNode,
                                CSMetaNode *functionMeta);

  JavaSourceGenerator m_sourceGenerator;
};


}