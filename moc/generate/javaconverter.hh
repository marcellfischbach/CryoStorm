//
// Created by MCEL on 07.10.2024.
//

#pragma once

#include <string>
#include <vector>
#include <set>


namespace cs::xml
{
class csElement;
}

class JavaConverterArgument
{
public:
  JavaConverterArgument(std::string jtype,
                        std::string id,
                        std::string suffix,
                        std::string javaType,
                        std::string jniTypeSig);
  [[nodiscard]] const std::string &GetJType() const;
  [[nodiscard]] const std::string &GetID() const;
  [[nodiscard]] const std::string &GetSuffix() const;
  [[nodiscard]] const std::string &GetJavaType() const;
  [[nodiscard]] const std::string &GetJniTypeSig() const;
  [[nodiscard]] const std::string &GetJniTypeSigMangled() const;
private:
  std::string m_jtype;
  std::string m_id;
  std::string m_suffix;
  std::string m_javaType;
  std::string m_jniTypeSig;
  std::string m_jniTypeSigMangled;
};

class JavaConverter
{
public:
  void AddType(const std::string &type);
  void SetJniName(const std::string &jniName);
  const std::string &GetJniName() const;
  const std::string &GetJniNameMangled() const;
  void AddInputArgument(const JavaConverterArgument &argument);
  void AddOutputArgument(const JavaConverterArgument &argument);

  void SetInputScript(const std::string &inputScript);
  void SetOutputScript(const std::string &outputScript);
  void SetOutputReturnType(const std::string &outputReturnType);

  void SetFullQualifiedType(const std::string &fullQualifiedType);
  const std::string &GetFullQualifiedType() const;

  bool IsValidForType(const std::string &type) const;

  const std::vector<JavaConverterArgument> &GetInputArguments() const;
  const std::vector<JavaConverterArgument> &GetOutputArguments() const;

  const std::string &GetOutputReturnType() const;


  std::string ConvertInput(size_t jniArgIdx, size_t csArgIdx) const;
  std::string ConvertOutput() const;

private:
  std::set<std::string> m_types;
  std::vector<JavaConverterArgument> m_inputArguments;
  std::vector<JavaConverterArgument> m_outputArguments;
  std::string m_outputReturnType;
  std::string m_fullQualifiedType;

  std::string m_inputScript;
  std::string m_outputScript;


};


class JavaConverters
{
public:
  static JavaConverters *Get();

  void ReadConverters(const std::string &path);
  const JavaConverter *FindConverter(const std::string &type) const;

private:
  JavaConverters() = default;


  void ReadPath(const std::string &path);
  void ReadConvertersFile(const std::string &filename);

  void ReadConverter(const cs::xml::csElement *converterElement);
  void ReadConverters(const cs::xml::csElement *element);

  void ReadTypes(JavaConverter &converter, const cs::xml::csElement *typesElement);
  void ReadInputArguments(JavaConverter &converter, const cs::xml::csElement *inputArgumentsElement);
  void ReadOutputArguments(JavaConverter &converter, const cs::xml::csElement *outputArgumentsElement);

private:

  std::vector<JavaConverter> m_converters;


};
