#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/math/csVector2f.hh>
#include <csCore/math/csVector3f.hh>
#include <csCore/math/csVector4f.hh>
#include <csCore/math/csVector2i.hh>
#include <csCore/resource/csAssetLocator.hh>

#include <string>
namespace cs
{

class csCryoFile;
class csCryoFileElement;

class CS_CORE_API csSettingsFile
{
public:
  csSettingsFile(const std::string& encodedLocator);
  csSettingsFile(const csAssetLocator& locator);
  ~csSettingsFile();

  CS_NODISCARD bool IsValid() const;


  CS_NODISCARD std::string GetText(const std::string& path, const std::string &defaultValue = std::string("")) const;
  CS_NODISCARD float GetFloat(const std::string& path, float defaultValue = 0.0f) const;
  CS_NODISCARD int GetInt(const std::string& path, int defaultValue = 0) const;
  CS_NODISCARD bool GetBool(const std::string& path, bool defaultValue = false) const;
  CS_NODISCARD csVector2f GetVector2f(const std::string& path, const csVector2f& defaultValue = csVector2f(0.0f, 0.0f)) const;
  CS_NODISCARD csVector3f GetVector3f(const std::string& path, const csVector3f& defaultValue = csVector3f(0.0f, 0.0f, 0.0f)) const;
  CS_NODISCARD csVector4f GetVector4f(const std::string& path, const csVector4f& defaultValue = csVector4f(0.0f, 0.0f, 0.0f, 0.0f)) const;
  CS_NODISCARD csVector2i GetVector2i(const std::string& path, const csVector2i& defaultValue = csVector2i(0, 0)) const;

private:
  void Initialize(const csAssetLocator &locator);
  CS_NODISCARD csCryoFileElement* GetElement(const std::string &path) const;

  csCryoFile * m_file;
};

class CS_CORE_API csSettings
{
public:
  static csSettings &Get();

  const csSettingsFile &Graphics() const;
  const csSettingsFile &Display() const;
private:
  csSettings();
  static csSettings * s_settings;

  csSettingsFile *m_graphics;
  csSettingsFile *m_display;
};

}

