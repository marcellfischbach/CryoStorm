
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


class CE_CORE_API CrimsonFileAttribute
{
public:
  enum class AttributeType {
    String,
    Number
  };
public:
  CrimsonFileAttribute(const std::string& value, AttributeType type);
  CrimsonFileAttribute(const std::string& name, const std::string& value, AttributeType type);

  CE_NODISCARD const std::string& GetName() const;
  CE_NODISCARD const std::string& GetValue() const;
  CE_NODISCARD int GetIntValue() const;
  CE_NODISCARD float GetFloatValue() const;
  CE_NODISCARD double GetDoubleValue() const;
  CE_NODISCARD AttributeType GetType() const;

private:

  std::string m_name;
  std::string m_value;
  AttributeType m_type;
};

class CE_CORE_API CrimsonFileElement
{
public:
  CrimsonFileElement();
  ~CrimsonFileElement();

  void SetTagName(const std::string& tagName);
  CE_NODISCARD const std::string& GetTagName() const;

  void AddChild(CrimsonFileElement* child);
  CE_NODISCARD CrimsonFileElement* GetParent();
  CE_NODISCARD const CrimsonFileElement* GetParent() const;

  CE_NODISCARD size_t GetNumberOfChildren() const;
  CE_NODISCARD CrimsonFileElement* GetChild(size_t idx);
  CE_NODISCARD const CrimsonFileElement* GetChild(size_t idx) const;
  CE_NODISCARD bool HasChild(const std::string& childName) const;
  CE_NODISCARD CrimsonFileElement* GetChild(const std::string& childName);
  CE_NODISCARD const CrimsonFileElement* GetChild(const std::string& childName) const;

  void AddAttribute(const CrimsonFileAttribute &attribute);

  CE_NODISCARD size_t GetNumberOfAttributes() const;
  CE_NODISCARD bool HasAttribute(const std::string& attributeName) const;
  CE_NODISCARD const CrimsonFileAttribute* GetAttribute(size_t idx) const;
  CE_NODISCARD const CrimsonFileAttribute* GetAttribute(const std::string &attributeName) const;

  CE_NODISCARD const std::string GetAttribute(size_t idx, const std::string &defaultValue) const;
  CE_NODISCARD const std::string GetAttribute(const std::string& attributeName, const std::string &defaultValue) const;

  CE_NODISCARD int GetAttribute(size_t idx, int defaultValue) const;
  CE_NODISCARD int GetAttribute(const std::string& attributeName, int defaultValue) const;

  CE_NODISCARD float GetAttribute(size_t idx, float defaultValue) const;
  CE_NODISCARD float GetAttribute(const std::string& attributeName, float defaultValue) const;


  CE_NODISCARD double GetAttribute(size_t idx, double defaultValue) const;
  CE_NODISCARD double GetAttribute(const std::string& attributeName, double defaultValue) const;



private:
  std::string m_tagName;


  std::vector<CrimsonFileElement*> m_children;
  CrimsonFileElement* m_parent;

  std::vector<CrimsonFileAttribute> m_attributes;


};


struct iBuffer;

class CE_CORE_API CrimsonFile
{
public:
  CrimsonFile();
  ~CrimsonFile();


  CE_NODISCARD bool Parse(const std::string& filename);
  CE_NODISCARD bool Parse(iFile* file);
  CE_NODISCARD bool Parse(const char* buffer, size_t bufferSize);

  std::string Print(bool format, int indent);

  CE_NODISCARD CrimsonFileElement* Root();
  CE_NODISCARD const CrimsonFileElement* Root() const;

  CE_NODISCARD const char* GetData() const;
  CE_NODISCARD size_t GetDataSize() const;

  void Debug() const;

private:
  CE_NODISCARD bool Parse(iBuffer* buffer);
  CrimsonFileElement m_root;

  char *m_data;
  size_t m_dataSize;

};


}