//
// Created by MCEL on 07.10.2024.
//

#include <generate/javaconverter.hh>
#include <vector>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <csXml/csXml.hh>

using namespace cs;

JavaConverters *JavaConverters::Get()
{
  static JavaConverters converter;
  return &converter;
}

const JavaConverter *JavaConverters::FindConverter(const std::string &type) const
{
  for (const auto &item: m_converters)
  {
    if (item.IsValidForType(type))
    {
      return &item;
    }
  }
  return nullptr;
}


void JavaConverters::ReadConverters(const std::string &paths)
{
  std::stringstream stream(paths);
  std::string       path;

  while (std::getline(stream, path, ';'))
  {
    ReadPath(path);
  }
}


void JavaConverters::ReadPath(const std::string &path)
{
  for (const auto &entry: std::filesystem::directory_iterator(path))
  {
    const std::filesystem::path &file_path = entry.path();
    const std::string           name       = file_path.string();


    // file ending -moc.xml
    if (name.length() < 8)
    {
      continue;
    }
    if (name.substr(name.length() - 8) != "-moc.xml")
    {
      continue;
    }

    ReadConvertersFile(name);
  }
}

void JavaConverters::ReadConvertersFile(const std::string &filename)
{
  try
  {
    xml::csDocument      *doc  = xml::csParser::ParseFilename(filename);
    const xml::csElement *root = doc->GetRoot();

    if (root->GetTagName() == "converter")
    {
      ReadConverter(root);
    }
    else if (root->GetTagName() == "converters")
    {
      ReadConverters(root);
    }

  }
  catch (const xml::csParseException &e)
  {
    std::cout << "Error Parsing " << filename << ": " << e.what() << " @ " << e.GetLine() + 1 << ":"
              << e.GetColumn() + 1 << "\n";
  }
}

void JavaConverters::ReadConverters(const cs::xml::csElement *convertersElement)
{
  for (int i = 0; i < convertersElement->GetNumberOfChildren(); ++i)
  {
    const xml::csElement *childElement = convertersElement->GetChild(i)->AsElement();
    if (!childElement || childElement->GetTagName() != "converter")
    {
      continue;
    }

    ReadConverter(childElement);
  }
}


void JavaConverters::ReadConverter(const cs::xml::csElement *converterElement)
{
  JavaConverter        converter;
  const xml::csElement *inputConversionElement;
  const xml::csElement *outputConversionElement;
  for (int             i = 0; i < converterElement->GetNumberOfChildren(); ++i)
  {
    const xml::csElement *childElement = converterElement->GetChild(i)->AsElement();
    if (!childElement)
    {
      continue;
    }

    if (childElement->GetTagName() == "types")
    {
      ReadTypes(converter, childElement);
    }
    else if (childElement->GetTagName() == "in-arguments")
    {
      ReadInputArguments(converter, childElement);
    }
    else if (childElement->GetTagName() == "out-arguments")
    {
      ReadOutputArguments(converter, childElement);
    }
    else if (childElement->GetTagName() == "in-conversion")
    {
      converter.SetInputScript(childElement->GetContent());
    }
    else if (childElement->GetTagName() == "out-conversion")
    {
      converter.SetFullQualifiedType(childElement->GetAttribute("fullQualifiedType"));
      converter.SetOutputReturnType(childElement->GetAttribute("returnType"));
      converter.SetOutputScript(childElement->GetContent());
    }
  }



  m_converters.push_back(converter);
}

void JavaConverters::ReadTypes(JavaConverter &converter, const cs::xml::csElement *typesElement)
{
  for (int i = 0; i < typesElement->GetNumberOfChildren(); ++i)
  {
    const xml::csElement *typeElement = typesElement->GetChild(i)->AsElement();
    if (typeElement->GetTagName() != "type")
    {
      continue;
    }

    if (typesElement->GetNumberOfChildren() > 0)
    {
      const xml::csText *typeText = typeElement->GetChild(0)->AsText();
      if (typeText)
      {
        converter.AddType(typeText->GetContent());
      }
    }
  }
}

void JavaConverters::ReadInputArguments(JavaConverter &converter, const cs::xml::csElement *inputArgumentsElement)
{
  for (int i = 0; i < inputArgumentsElement->GetNumberOfChildren(); ++i)
  {
    const xml::csElement *inputArgumentElement = inputArgumentsElement->GetChild(i)->AsElement();
    if (inputArgumentElement && inputArgumentElement->GetTagName() == "argument"
        && inputArgumentElement->HasAttribute("jtype")
        && inputArgumentElement->HasAttribute("id"))
    {
      JavaConverterArgument argument(inputArgumentElement->GetAttribute("jtype"),
                                     inputArgumentElement->GetAttribute("id"),
                                     inputArgumentElement->GetAttribute("suffix"),
                                     inputArgumentElement->GetAttribute("javaType")
                                     );
      converter.AddInputArgument(argument);
    }
  }
}


void JavaConverters::ReadOutputArguments(JavaConverter &converter, const cs::xml::csElement *outputArgumentsElement)
{
  for (int i = 0; i < outputArgumentsElement->GetNumberOfChildren(); ++i)
  {
    const xml::csElement *outputArgumentElement = outputArgumentsElement->GetChild(i)->AsElement();
    if (outputArgumentElement && outputArgumentElement->GetTagName() == "argument"
        && outputArgumentElement->HasAttribute("jtype")
        && outputArgumentElement->HasAttribute("id"))
    {
      JavaConverterArgument argument(outputArgumentElement->GetAttribute("jtype"),
                                     outputArgumentElement->GetAttribute("id"),
                                     outputArgumentElement->GetAttribute("suffix"),
                                     outputArgumentElement->GetAttribute("javaType")
                                     );
      converter.AddOutputArgument(argument);
    }
  }
}


JavaConverterArgument::JavaConverterArgument(std::string jtype, std::string id, std::string suffix, std::string javaType)
    : m_jtype(std::move(jtype))
    , m_id(std::move(id))
    , m_suffix(std::move(suffix))
    , m_javaType(std::move(javaType))
{

}

const std::string &JavaConverterArgument::GetJType() const
{
  return m_jtype;
}

const std::string &JavaConverterArgument::GetID() const
{
  return m_id;
}

const std::string &JavaConverterArgument::GetSuffix() const
{
  return m_suffix;
}

const std::string &JavaConverterArgument::GetJavaType() const
{
  return m_javaType;
}

void JavaConverter::AddType(const std::string &type)
{
  m_types.insert(type);
}

void JavaConverter::AddInputArgument(const JavaConverterArgument &argument)
{
  m_inputArguments.push_back(argument);
}

void JavaConverter::AddOutputArgument(const JavaConverterArgument &argument)
{
  m_outputArguments.push_back(argument);
}

void JavaConverter::SetInputScript(const std::string &inputScript)
{
  m_inputScript = inputScript;
}

void JavaConverter::SetOutputScript(const std::string &outputScript)
{
  m_outputScript = outputScript;
}

void JavaConverter::SetOutputReturnType(const std::string &outputReturnType)
{
  m_outputReturnType = outputReturnType;
}

void JavaConverter::SetFullQualifiedType(const std::string &fullQualifiedType)
{
  m_fullQualifiedType = fullQualifiedType;
}

const std::string &JavaConverter::GetFullQualifiedType() const
{
  return m_fullQualifiedType;
}

bool JavaConverter::IsValidForType(const std::string &type) const
{
  return m_types.find(type) != m_types.end();
}

const std::vector<JavaConverterArgument> &JavaConverter::GetInputArguments() const
{
  return m_inputArguments;
}


const std::vector<JavaConverterArgument> &JavaConverter::GetOutputArguments() const
{
  return m_outputArguments;
}

const std::string &JavaConverter::GetOutputReturnType() const
{
  return m_outputReturnType;
}

std::string replace (const std::string &text, const std::string &ref, const std::string &replacement)
{
  std::string result = text;
  while (true)
  {
    unsigned __int64 i = result.find(ref);
    if (i == std::string::npos)
    {
      break;
    }
    
    result = result.replace(i, ref.size(), replacement.c_str(), replacement.length());
  }
  return result;
}


std::string JavaConverter::ConvertInput(size_t jniArgIdx, size_t csArgIdx) const
{
  std::string script = m_inputScript; 
  script = replace (script, "${csArg}", "csArg" + std::to_string(csArgIdx));
  script = replace (script, "${csTmp}", "csTmp" + std::to_string(csArgIdx));
  for (size_t i=0; i<m_inputArguments.size(); i++)
  {
    const JavaConverterArgument &argument = m_inputArguments[i];
    script = replace(script, "${" + argument.GetID() + "}", "jniArg" + std::to_string(jniArgIdx + i));
  }
  return script;
}


std::string JavaConverter::ConvertOutput() const
{
  std::string script = m_outputScript;
  script = replace (script, "${csRet}", "csReturnValue");
  script = replace (script, "${csTmp}", "csReturnTmp");
  for (size_t i=0; i<m_outputArguments.size(); i++)
  {
    const JavaConverterArgument &argument = m_outputArguments[i];
    script = replace(script, "${" + argument.GetID() + "}", "jniOutArg" + std::to_string(i));
  }
  return script;
}