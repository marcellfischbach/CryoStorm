
#pragma once

#include <csCryoFile/csCryoFileExport.hh>
#include <csCore/csDefs.hh>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace cs::file
{


class CS_CRYOFILE_API csCryoFileAttribute
{
public:
  enum class AttributeType
  {
    String,
    Number
  };
public:
  csCryoFileAttribute(const std::string &value, AttributeType type);
  csCryoFileAttribute(const std::string &name, const std::string &value, AttributeType type);

  CS_NODISCARD const std::string &GetName() const;
  CS_NODISCARD const std::string &GetValue() const;
  CS_NODISCARD int GetIntValue() const;
  CS_NODISCARD float GetFloatValue() const;
  CS_NODISCARD double GetDoubleValue() const;
  CS_NODISCARD AttributeType GetType() const;

private:

  std::string   m_name;
  std::string   m_value;
  AttributeType m_type;
};

class CS_CRYOFILE_API csCryoFileElement
{
public:
  csCryoFileElement();
  csCryoFileElement(const std::string &tagName, csCryoFileElement *parent = nullptr);
  ~csCryoFileElement();

  void Write(std::ostream& out, bool format, const std::string &indent, unsigned indentDepth = 2);

  void SetTagName(const std::string &tagName);
  CS_NODISCARD const std::string &GetTagName() const;

  void AddChild(csCryoFileElement *child);
  CS_NODISCARD csCryoFileElement *GetParent();
  CS_NODISCARD const csCryoFileElement *GetParent() const;

  CS_NODISCARD size_t GetNumberOfChildren() const;
  CS_NODISCARD csCryoFileElement *GetChild(size_t idx);
  CS_NODISCARD const csCryoFileElement *GetChild(size_t idx) const;
  CS_NODISCARD bool HasChild(const std::string &childName) const;
  CS_NODISCARD csCryoFileElement *GetChild(const std::string &childName);
  CS_NODISCARD const csCryoFileElement *GetChild(const std::string &childName) const;

  void AddAttribute(const csCryoFileAttribute &attribute);
  void AddAttribute(const std::string& value);
  void AddAttribute(const std::string& name, const std::string& value);
  void AddStringAttribute(const std::string& value);
  void AddStringAttribute(const std::string &name, const std::string& value);

  CS_NODISCARD size_t GetNumberOfAttributes() const;
  CS_NODISCARD bool HasAttribute(const std::string &attributeName) const;
  CS_NODISCARD const csCryoFileAttribute *GetAttribute(size_t idx) const;
  CS_NODISCARD const csCryoFileAttribute *GetAttribute(const std::string &attributeName) const;

  CS_NODISCARD const std::string GetAttribute(size_t idx, const std::string &defaultValue) const;
  CS_NODISCARD const std::string GetAttribute(const std::string &attributeName, const std::string &defaultValue) const;

  CS_NODISCARD int GetAttribute(size_t idx, int defaultValue) const;
  CS_NODISCARD int GetAttribute(const std::string &attributeName, int defaultValue) const;

  CS_NODISCARD float GetAttribute(size_t idx, float defaultValue) const;
  CS_NODISCARD float GetAttribute(const std::string &attributeName, float defaultValue) const;


  CS_NODISCARD double GetAttribute(size_t idx, double defaultValue) const;
  CS_NODISCARD double GetAttribute(const std::string &attributeName, double defaultValue) const;

private:
  std::string m_tagName;


  std::vector<csCryoFileElement *> m_children;
  csCryoFileElement *m_parent;

  std::vector<csCryoFileAttribute> m_attributes;


};


struct iBuffer;

class CS_CRYOFILE_API csCryoFile
{
public:
  csCryoFile();
  ~csCryoFile();


  CS_NODISCARD bool Parse(const std::string &filename);
  CS_NODISCARD bool Parse(const std::vector<char>& data);
  CS_NODISCARD bool Parse(const char *buffer, size_t bufferSize);
  void Write(std::ostream& out, bool format = true, unsigned indent = 2);

  CS_NODISCARD csCryoFileElement *Root();
  CS_NODISCARD const csCryoFileElement *Root() const;

  CS_NODISCARD std::vector<std::string> GetDataNames() const;
  CS_NODISCARD size_t GetNumberOfDatas() const;
  CS_NODISCARD const std::vector<uint8_t>& GetData(size_t idx) const;
  CS_NODISCARD const std::vector<uint8_t>& GetData(const std::string &name) const;

  void AddData(const std::string& name, uint32_t size, uint8_t* data);
  void AddData(const std::string& name, std::vector<uint8_t> data);

  void Debug() const;



private:

  void WriteData(std::ostream& out);

  CS_NODISCARD bool Parse(iBuffer *buffer);
  void ParseData(size_t size, char* buffer);
  void ParseData_V1(std::istream &in);
  csCryoFileElement m_root;


  struct Data
  {
    std::string name;
    std::vector<uint8_t> data;
  };

  std::vector<Data> m_datas;

};


}