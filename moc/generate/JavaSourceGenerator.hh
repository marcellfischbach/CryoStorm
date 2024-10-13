
#pragma once

#include <string>
#include <list>

class JavaSourceGenerator
{
public:
  static void SetBasePath (std::string basePath);
  static bool ShouldGenerate ();

  void BeginClass (const std::string &fqClassName);

  void BeginFunction (const std::string &cppFunctionName, const std::string &jniReturnType);
  void AddParameter (const std::string &type, const std::string &name, const std::string &comment);
  void EndFunction ();

  void EndClass();

private:
  void Parse();
  std::string GenerateClassBegin();
  std::string GenerateClassEnd();

  std::string ReadFile();
  std::string ReadClassBegin(const std::string &content);
  std::string ReadClassEnd(const std::string &content);

  std::string m_fqClassName;

  std::string m_absFileName;
  std::string m_nativeCodeFragments;
  std::string m_package;
  std::string m_className;
  static std::string s_basePath;


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
