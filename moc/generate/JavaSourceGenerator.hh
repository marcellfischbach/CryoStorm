
#pragma once

#include <string>
#include <list>

namespace cs::moc
{
class ClassNode;
class CSMetaNode;
class FunctionNode;
}

class JavaSourceGenerator
{
public:
  static void SetBasePath (std::string basePath);
  static bool ShouldGenerate ();

  void BeginClass (cs::moc::ClassNode* classNode, cs::moc::CSMetaNode *classMeta, const std::string &cppClassName, const std::string &fqClassName);

  void BeginFunction (cs::moc::FunctionNode *functionNode, cs::moc::CSMetaNode *functionMeta, const std::string &cppFunctionName, const std::string &jniReturnType);
  void AddParameter (const std::string &type, const std::string &name, const std::string &comment);
  void EndFunction ();

  void EndClass();

private:
  void Parse();
  cs::moc::ClassNode* m_classNode;
  cs::moc::CSMetaNode *m_classMeta;

  std::string GenerateClassBegin();
  std::string GenerateClassEnd();

  std::string ReadFile();
  std::string ReadClassBegin(const std::string &content);
  std::string ReadClassEnd(const std::string &content);

  std::string m_fqClassName;
  std::string m_cppClassName;

  std::string m_relFileName;
  std::string m_absFileName;
  std::string m_nativeCodeFragments;
  std::string m_package;
  std::string m_className;
  static std::string s_basePath;

  cs::moc::FunctionNode *m_functionNode;
  cs::moc::CSMetaNode *m_functionMeta;
  std::string m_functionType;
  std::string m_functionName;
  struct Argument
  {
    std::string typeName;
    std::string name;
    std::string comment;
  };
  std::list<Argument> m_functionArguments;

};
