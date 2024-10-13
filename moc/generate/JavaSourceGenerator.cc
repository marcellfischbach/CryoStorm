
#include <generate/JavaSourceGenerator.hh>
#include <list>
#include <iostream>
#include <fstream>
#include <filesystem>

std::string JavaSourceGenerator::s_basePath;

void JavaSourceGenerator::SetBasePath(std::string basePath)
{
  s_basePath = std::move(basePath);
}

bool JavaSourceGenerator::ShouldGenerate()
{
  return !s_basePath.empty();
}

std::string class_to_file_name(const std::string &fqClassName)
{
  std::string res;
  for (auto ch: fqClassName)
  {
    if (ch == '"')
    {
      continue;
    }
    if (ch == '.')
    {
      res += "/";
    }
    else
    {
      res += ch;
    }
  }

  return res + ".java";
}


std::string package_to_path(const std::string &package)
{
  std::string res;
  for (auto ch: package)
  {
    if (ch == '"')
    {
      continue;
    }
    if (ch == '.')
    {
      res += "/";
    }
    else
    {
      res += ch;
    }
  }

  return res;
}


void JavaSourceGenerator::BeginClass(const std::string &fqClassName)
{
  if (!ShouldGenerate())
  {
    return;
  }
  m_fqClassName = fqClassName;
  Parse();
}

void JavaSourceGenerator::Parse()
{
  size_t i = m_fqClassName.find_last_of('.');
  if (i == std::string::npos)
  {
    m_package = "";
    m_className = m_fqClassName;
  }
  else
  {
    m_package = m_fqClassName.substr(0, i);
    m_className = m_fqClassName.substr(i + 1);
  }

  m_absFileName = s_basePath + "/" + class_to_file_name(m_fqClassName);
  m_nativeCodeFragments = "";
  std::cout << "Filename: " << m_absFileName << std::endl;
  std::cout << "Package : " << m_package << std::endl;
  std::cout << "Class   : " << m_className << std::endl;
}

inline bool exists(const std::string &name)
{
  std::ifstream f(name.c_str());
  return f.good();
}


void JavaSourceGenerator::EndClass()
{
  if (!ShouldGenerate())
  {
    return;
  }

  std::string begin, end;
  if (exists(m_absFileName))
  {
    std::string content = ReadFile();
    begin = ReadClassBegin(content);
    end = ReadClassEnd(content);
  }

  if (begin.empty() || end.empty())
  {
    begin = GenerateClassBegin();
    end = GenerateClassEnd();
  }


//  std::cout << begin << std::endl
//            << m_nativeCodeFragments << std::endl
//            << end << std::endl;


  std::string packagePath = package_to_path(m_package);
  std::filesystem::path path(s_basePath + "/" + packagePath);
  std::filesystem::create_directories(path);
  std::ofstream outputFile(m_absFileName);
  if (outputFile.is_open())
  {
    outputFile << begin << std::endl
               << m_nativeCodeFragments << std::endl
               << end << std::endl;
    outputFile.close();
  }
  else
  {
    std::cout << "Unable to open " << m_absFileName << std::endl;
  }
}


static std::string s_beginMarker = "//##BEGIN-csMOC # Don't remove";
static std::string s_endMarker = "//##END-csMOC # Don't remove";

std::string JavaSourceGenerator::GenerateClassBegin()
{
  std::string source;

  if (!m_package.empty())
  {
    source += "package " + m_package + ";\n\n";
  }

  source += "public class " + m_className + "\n";
  source += "{\n";
  source += s_beginMarker;
  return source;
}

std::string JavaSourceGenerator::GenerateClassEnd()
{
  std::string source;
  source += s_endMarker + "\n";
  source += "}\n";
  source += "\n";
  return source;
}

std::string JavaSourceGenerator::ReadFile()
{
  std::ifstream inputFile(m_absFileName);
  if (!inputFile.is_open())
  {
    return "";
  }

  std::string content;
  std::string line;
  while (std::getline(inputFile, line))
  {
    content += line + "\n";
  }

  inputFile.close();
  return content;
}


std::string JavaSourceGenerator::ReadClassBegin(const std::string &content)
{
  size_t i = content.find(s_beginMarker);
  if (i == std::string::npos)
  {
    return "";
  }

  return content.substr(0, i + s_beginMarker.length());
}

std::string JavaSourceGenerator::ReadClassEnd(const std::string &content)
{
  size_t i = content.find(s_beginMarker);
  if (i == std::string::npos)
  {
    return "";
  }
  i = content.find(s_endMarker);
  if (i == std::string::npos)
  {
    return "";
  }

  return content.substr(i);
}

std::string map_jni_type(const std::string &jniType)
{
  if (jniType == "jbooleanArray")
  {
    return "boolean[]";
  }
  else if (jniType == "jbyteArray")
  {
    return "byte[]";
  }
  else if (jniType == "jcharArray")
  {
    return "char[]";
  }
  else if (jniType == "jshortArray")
  {
    return "short[]";
  }
  else if (jniType == "intArray")
  {
    return "int[]";
  }
  else if (jniType == "longArray")
  {
    return "long[]";
  }
  else if (jniType == "jfloatArray")
  {
    return "float[]";
  }
  else if (jniType == "jdoubleArray")
  {
    return "double[]";
  }
  else if (jniType == "jobject")
  {
    return "Object";
  }
  if (!jniType.empty() && jniType[0] == 'j')
  {
    return jniType.substr(1);
  }
  return jniType;
}

std::string map_cpp_functionName (const std::string &cppFunctionName)
{
  return "n" + cppFunctionName;
}


void JavaSourceGenerator::BeginFunction(const std::string &cppFunctionName, const std::string &jniReturnType)
{

  m_functionType = map_jni_type(jniReturnType);
  m_functionName = map_cpp_functionName(cppFunctionName);
}

void JavaSourceGenerator::AddParameter(const std::string &type, const std::string &name, const std::string &comment)
{
  m_functionArguments.emplace_back(map_jni_type(type), name, comment);
}

void JavaSourceGenerator::EndFunction()
{
  std::string functionDeclaration;

  std::string decl = "  private static native " + m_functionType + " " + m_functionName + "(";
  std::string space (decl.size(), ' ');
  m_nativeCodeFragments += decl;
  m_nativeCodeFragments += "long ref /* this ptr */";
  for (const auto &item: m_functionArguments)
  {
    m_nativeCodeFragments += ",\n" + space + item.typeName + " " + item.name;
    if (!item.comment.empty())
    {
      m_nativeCodeFragments += " /* " + item.comment + " */";
    }
  }
  m_nativeCodeFragments += "\n" + space + ");\n\n";

  // cleanup
  m_functionName = "";
  m_functionType = "";
  m_functionArguments.clear();
}
