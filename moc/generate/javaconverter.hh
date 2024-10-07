//
// Created by MCEL on 07.10.2024.
//

#pragma once

#include <string>
#include <vector>
#include <set>


namespace cryo::xml
{
class csElement;
}

class JavaConverterArgument
{
public:
  JavaConverterArgument(const std::string &jtype, const std::string &id);
  [[nodiscard]] const std::string &GetJType () const;
  [[nodiscard]] const std::string &GetID () const;
private:
  std::string m_jtype;
  std::string m_id;
};

class JavaConverter
{
public:
  void AddType (const std::string &type);
  void AddInputArgument (const JavaConverterArgument &argument);
  void AddOutputArgument (const JavaConverterArgument &argument);

  void SetInputScript (const std::string &inputScript);
  void SetOutputScript (const std::string &outputScript);
  void SetOutputReturnType (const std::string &outputReturnType);

  void SetFullQualifiedType (const std::string &fullQualifiedType);
  const std::string &GetFullQualifiedType() const;

  bool IsValidForType (const std::string &type) const;

  const std::vector<JavaConverterArgument> &GetInputArguments () const;
  const std::vector<JavaConverterArgument> &GetOutputArguments () const;

  const std::string &GetOutputReturnType () const;


  std::string ConvertInput (size_t jniArgIdx, size_t csArgIdx) const;
  std::string ConvertOutput () const;

private:

  std::set<std::string>              m_types;
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
  const JavaConverter *FindConverter (const std::string &type) const;

private:
  JavaConverters() = default;


  void ReadPath(const std::string &path);
  void ReadConvertersFile(const std::string &filename);

  void ReadConverter(const cryo::xml::csElement *converterElement);
  void ReadConverters (const cryo::xml::csElement *element);

  void ReadTypes (JavaConverter &converter, const cryo::xml::csElement* typesElement);
  void ReadInputArguments (JavaConverter &converter, const cryo::xml::csElement* inputArgumentsElement);
  void ReadOutputArguments (JavaConverter &converter, const cryo::xml::csElement* outputArgumentsElement);

private:

  std::vector<JavaConverter> m_converters;


};
