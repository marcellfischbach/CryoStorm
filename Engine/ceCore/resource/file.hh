
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/defs.hh>
#include <string>
#include <vector>

namespace ce
{
struct iFile;
}

namespace ce
{


class CS_CORE_API CrimsonFileAttribute
{
public:
  enum class AttributeType {
    String,
    Number
  };
public:
  CrimsonFileAttribute(const std::string& value, AttributeType type);
  CrimsonFileAttribute(const std::string& name, const std::string& value, AttributeType type);

  CS_NODISCARD const std::string& GetName() const;
  CS_NODISCARD const std::string& GetValue() const;
  CS_NODISCARD int GetIntValue() const;
  CS_NODISCARD float GetFloatValue() const;
  CS_NODISCARD double GetDoubleValue() const;
  CS_NODISCARD AttributeType GetType() const;

private:

  std::string m_name;
  std::string m_value;
  AttributeType m_type;
};

class CS_CORE_API CrimsonFileElement
{
public:
  CrimsonFileElement();
  ~CrimsonFileElement();

  void SetTagName(const std::string& tagName);
  CS_NODISCARD const std::string& GetTagName() const;

  void AddChild(CrimsonFileElement* child);
  CS_NODISCARD CrimsonFileElement* GetParent();
  CS_NODISCARD const CrimsonFileElement* GetParent() const;

  CS_NODISCARD size_t GetNumberOfChildren() const;
  CS_NODISCARD CrimsonFileElement* GetChild(size_t idx);
  CS_NODISCARD const CrimsonFileElement* GetChild(size_t idx) const;
  CS_NODISCARD bool HasChild(const std::string& childName) const;
  CS_NODISCARD CrimsonFileElement* GetChild(const std::string& childName);
  CS_NODISCARD const CrimsonFileElement* GetChild(const std::string& childName) const;

  void AddAttribute(const CrimsonFileAttribute &attribute);

  CS_NODISCARD size_t GetNumberOfAttributes() const;
  CS_NODISCARD bool HasAttribute(const std::string& attributeName) const;
  CS_NODISCARD const CrimsonFileAttribute* GetAttribute(size_t idx) const;
  CS_NODISCARD const CrimsonFileAttribute* GetAttribute(const std::string &attributeName) const;

  CS_NODISCARD const std::string GetAttribute(size_t idx, const std::string &defaultValue) const;
  CS_NODISCARD const std::string GetAttribute(const std::string& attributeName, const std::string &defaultValue) const;

  CS_NODISCARD int GetAttribute(size_t idx, int defaultValue) const;
  CS_NODISCARD int GetAttribute(const std::string& attributeName, int defaultValue) const;

  CS_NODISCARD float GetAttribute(size_t idx, float defaultValue) const;
  CS_NODISCARD float GetAttribute(const std::string& attributeName, float defaultValue) const;


  CS_NODISCARD double GetAttribute(size_t idx, double defaultValue) const;
  CS_NODISCARD double GetAttribute(const std::string& attributeName, double defaultValue) const;



private:
  std::string m_tagName;


  std::vector<CrimsonFileElement*> m_children;
  CrimsonFileElement* m_parent;

  std::vector<CrimsonFileAttribute> m_attributes;


};


struct iBuffer;

class CS_CORE_API CrimsonFile
{
public:
  CrimsonFile();
  ~CrimsonFile();


  CS_NODISCARD bool Parse(const std::string& filename);
  CS_NODISCARD bool Parse(iFile* file);
  CS_NODISCARD bool Parse(const char* buffer, size_t bufferSize);

  std::string Print(bool format, int indent);

  CS_NODISCARD CrimsonFileElement* Root();
  CS_NODISCARD const CrimsonFileElement* Root() const;

  CS_NODISCARD const char* GetData() const;
  CS_NODISCARD size_t GetDataSize() const;

  void Debug() const;

private:
  CS_NODISCARD bool Parse(iBuffer* buffer);
  CrimsonFileElement m_root;

  char *m_data;
  size_t m_dataSize;

};


}