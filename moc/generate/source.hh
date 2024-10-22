
#pragma once


#include <generate/generator.hh>
#include <vector>
#include <list>

namespace cs::moc
{

struct iOutput;
class CSMetaNode;
class Argument;
class ClassNode;
class FunctionNode;
class MemberNode;
class TypeDef;

class SourceGenerator : public Generator
{
public:
  SourceGenerator();

  void Output(iOutput *output);

private:
};

class ClassGenerator
{
public:
  std::string OutputClass(ClassNode* classNode, std::list<NamespaceNode*> &nss, CSMetaNode* metaNode);

private:
  std::string GenerateAttribute(ClassNode* classNode, std::list<NamespaceNode*>& nss, MemberNode* memberNode, const std::string &visibility, CSMetaNode *metaNode);
  std::string GenerateTypeForAttribute(const TypeDef& typeDef);
  std::string GenerateInputTypeForAttribute(const TypeDef& typeDef);


  std::string GenerateFunctionClass(ClassNode *classNode, std::list<NamespaceNode*>& nss, FunctionNode* functionNode, const std::string& visibility, CSMetaNode *meta);
  std::string GenerateFunctionVoidInvokeMethod(ClassNode* classNode, FunctionNode* functionNode, bool constMethod);
  std::string GenerateFunctionValueInvokeMethod(ClassNode* classNode, FunctionNode* functionNode, bool constMethod);
  std::string GenerateFunctionReferenceInvokeMethod(ClassNode* classNode, FunctionNode* functionNode, bool constReturn, bool constMethod);
  std::string GenerateFunctionPointerInvokeMethod(ClassNode* classNode, FunctionNode* functionNode, bool constReturn, bool constMethod);


  std::string GenerateTypeForMethodInvokation(const TypeDef& typeDef);
  std::string GenerateTypeForMethodReturnInvokation(const TypeDef& typeDef);


  std::string GenerateCSValueDeclaration(const TypeDef& typeDef, bool withSubTypes = true);
  std::string GenerateAddAttribute(const Argument& argument);

  std::string GenerateClass(ClassNode* classNode, std::list<NamespaceNode*>& nss, CSMetaNode *meta);
  std::string GenerateQueryClass(ClassNode* classNode, std::list<NamespaceNode*>& nss, CSMetaNode* meta, bool _const);
  std::string GenerateCreateJObject(ClassNode* classNode, std::list<NamespaceNode*>& nss, CSMetaNode* meta);


private:
  uint32_t m_functionCounter = 0;
  std::vector<std::string> m_propertyClasses;
  std::vector<std::string> m_functionClasses;
};


}
