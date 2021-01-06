
#pragma once

#include <spcFile/fileexport.hh>
#include <string>
#include <vector>

namespace spc::file
{


class SPC_FILE_API Attribute
{
public:
  enum class AttributeType {
    String,
    Number
  };
public:
  Attribute(const std::string& value, AttributeType type);
  Attribute(const std::string& name, const std::string& value, AttributeType type);

  const std::string& GetName() const;
  const std::string& GetValue() const;
  AttributeType GetType() const;

private:

  std::string m_name;
  std::string m_value;
  AttributeType m_type;
};

class SPC_FILE_API Element
{
public:
  Element();
  ~Element();

  void SetTagName(const std::string& tagName);
  const std::string& GetTagName() const;

  void AddChild(Element* child);
  Element* GetParent();
  const Element* GetParent() const;

  size_t GetNumberOfChildren() const;
  Element* GetChild(size_t idx);
  const Element* GetChild(size_t idx) const;

  void AddAttribute(const Attribute &attribute);

  size_t GetNumberOfAttributes() const;
  const Attribute* GetAttribute(size_t idx) const;
  bool HasAttribute(const std::string& attributeName) const;
  const Attribute* GetAttribute(const std::string &attributeName) const;

private:
  std::string m_tagName;


  std::vector<Element*> m_children;
  Element* m_parent;

  std::vector<Attribute> m_attributes;

};


struct iBuffer;

class SPC_FILE_API File
{
public:
  File();
  ~File();


  bool Parse(const std::string& filename);
  bool Parse(const char* buffer, size_t bufferSize);

  std::string Print(bool format, int indent);
  void Print(const std::string& filename, bool format, int indent);
  void Print(char** buffer, size_t &bufferSize, bool format, int indent);

  Element* Root();
  const Element* Root() const;

  const char* GetData() const;
  size_t GetDataSize() const;

  void Debug() const;

private:
  bool Parse(iBuffer* buffer);
  Element m_root;

  char *m_data;
  size_t m_dataSize;

};


}