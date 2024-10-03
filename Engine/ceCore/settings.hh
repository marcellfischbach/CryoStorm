#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/math/vector2f.hh>
#include <ceCore/math/vector3f.hh>
#include <ceCore/math/vector4f.hh>
#include <ceCore/math/vector2i.hh>
#include <ceCore/resource/resourcelocator.hh>

#include <string>
namespace ce
{

class CrimsonFile;
class CrimsonFileElement;

class CS_CORE_API SettingsFile
{
public:
  SettingsFile(const std::string& encodedLocator);
  SettingsFile(const ResourceLocator& locator);
  ~SettingsFile();

  CS_NODISCARD bool IsValid() const;


  CS_NODISCARD std::string GetText(const std::string& path, const std::string &defaultValue = std::string("")) const;
  CS_NODISCARD float GetFloat(const std::string& path, float defaultValue = 0.0f) const;
  CS_NODISCARD int GetInt(const std::string& path, int defaultValue = 0) const;
  CS_NODISCARD bool GetBool(const std::string& path, bool defaultValue = false) const;
  CS_NODISCARD Vector2f GetVector2f(const std::string& path, const Vector2f& defaultValue = Vector2f(0.0f, 0.0f)) const;
  CS_NODISCARD Vector3f GetVector3f(const std::string& path, const Vector3f& defaultValue = Vector3f(0.0f, 0.0f, 0.0f)) const;
  CS_NODISCARD Vector4f GetVector4f(const std::string& path, const Vector4f& defaultValue = Vector4f(0.0f, 0.0f, 0.0f, 0.0f)) const;
  CS_NODISCARD Vector2i GetVector2i(const std::string& path, const Vector2i& defaultValue = Vector2i(0, 0)) const;

private:
  void Initialize(const ResourceLocator &locator);
  CS_NODISCARD CrimsonFileElement* GetElement(const std::string &path) const;

  CrimsonFile* m_file;
};

class CS_CORE_API Settings
{
public:
  static Settings &Get();

  const SettingsFile &Graphics() const;
  const SettingsFile &Display() const;
private:
  Settings ();
  static Settings* s_settings;

  SettingsFile *m_graphics;
  SettingsFile *m_display;
};

}

