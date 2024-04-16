#include <ceCore/settings.hh>
#include <ceCore/resource/file.hh>
#include <ceCore/resource/resourcelocator.hh>
#include <ceCore/resource/vfs.hh>
#

namespace ce
{

SettingsFile::SettingsFile(const std::string& locator)
  : m_file(nullptr)
{
  Initialize(ResourceLocator(locator));
}

SettingsFile::SettingsFile(const ResourceLocator& locator)
  : m_file(nullptr)
{
  Initialize(locator);
}

void SettingsFile::Initialize(const ResourceLocator& locator)
{
  iFile* file = VFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (file)
  {
    m_file = new CrimsonFile();
    if (!m_file->Parse(file))
    {
      delete m_file;
      m_file = nullptr;
    }
    file->Release();
  }
}

SettingsFile::~SettingsFile()
{
  delete m_file;
  m_file = nullptr;
}

bool SettingsFile::IsValid() const
{
  return m_file;
}

std::string SettingsFile::GetText(const std::string& path, const std::string& defaultValue) const
{
  CrimsonFileElement* element = GetElement(path);
  if (element)
  {
    return element->GetAttribute(0, "");
  }
  return defaultValue;
}

float SettingsFile::GetFloat(const std::string& path, float defaultValue) const
{
  CrimsonFileElement* element = GetElement(path);
  if (element)
  {
    return (float)atof(element->GetAttribute(0, "").c_str());
  }
  return defaultValue;
}

int SettingsFile::GetInt(const std::string& path, int defaultValue) const
{
  CrimsonFileElement* element = GetElement(path);
  if (element)
  {
    return atoi(element->GetAttribute(0, "").c_str());
  }
  return defaultValue;
}

bool SettingsFile::GetBool(const std::string& path, bool defaultValue) const
{
  std::string v = GetText(path, defaultValue ? "true" : "false");
  return std::string("true") == v
    || std::string("on") == v
    || std::string("yes") == v
    || std::string("positiv") == v
    || std::string("1") == v;
}

Vector2f SettingsFile::GetVector2f(const std::string& path, const Vector2f& defaultValue) const
{
  CrimsonFileElement* element = GetElement(path);
  if (element && element->GetNumberOfAttributes() >= 2)
  {
    return Vector2f(
      (float)element->GetAttribute(0)->GetDoubleValue(),
      (float)element->GetAttribute(1)->GetDoubleValue()
    );
  }
  return defaultValue;
}

Vector3f SettingsFile::GetVector3f(const std::string& path, const Vector3f& defaultValue) const
{
  CrimsonFileElement* element = GetElement(path);
  if (element)
  {
    if (element->GetNumberOfAttributes() >= 3)
    {
      return Vector3f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        (float)element->GetAttribute(2)->GetDoubleValue()
      );
    }
    else if (element->GetNumberOfAttributes() >= 2)
    {
      return Vector3f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        defaultValue.z
      );
    }
    else if (element->GetNumberOfAttributes() >= 1)
    {
      return Vector3f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        defaultValue.y,
        defaultValue.z
      );
    }
  }
  return defaultValue;
}


Vector4f SettingsFile::GetVector4f(const std::string& path, const Vector4f& defaultValue) const
{
  CrimsonFileElement* element = GetElement(path);
  if (element)
  {
    if (element->GetNumberOfAttributes() >= 4)
    {
      return Vector4f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        (float)element->GetAttribute(2)->GetDoubleValue(),
        (float)element->GetAttribute(3)->GetDoubleValue()
      );
    }
    else if (element->GetNumberOfAttributes() >= 3)
    {
      return Vector4f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        (float)element->GetAttribute(2)->GetDoubleValue(),
        defaultValue.w
      );
    }
    else if (element->GetNumberOfAttributes() >= 2)
    {
      return Vector4f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        defaultValue.z,
        defaultValue.w
      );
    }
    else if (element->GetNumberOfAttributes() >= 1)
    {
      return Vector4f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        defaultValue.y,
        defaultValue.z,
        defaultValue.w
      );
    }

  }
  return defaultValue;
}


Vector2i SettingsFile::GetVector2i(const std::string& path, const Vector2i& defaultValue) const
{
  CrimsonFileElement* element = GetElement(path);
  if (element && element->GetNumberOfAttributes() >= 2)
  {
    return Vector2i(
      element->GetAttribute(0)->GetIntValue(),
      element->GetAttribute(1)->GetIntValue()
    );
  }
  return defaultValue;
}
CrimsonFileElement* SettingsFile::GetElement(const std::string& path) const
{
  return m_file ? m_file->Root()->GetChild(path) : nullptr;
}


Settings* Settings::s_settings = nullptr;

Settings &Settings::Get()
{
  if (!s_settings)
  {
    s_settings = new Settings();
  }
  return *s_settings;
}

Settings::Settings()
{
  SettingsFile master ("file:///settings.config");

  if (master.IsValid())
  {
    m_graphics = new SettingsFile(master.GetText("graphics"));
    m_display  = new SettingsFile(master.GetText("display"));
  }
  else
  {
    m_graphics = new SettingsFile("file:///${config}/graphics.config");
    m_display = new SettingsFile("file:///${config}/display.config");
  }
}

const SettingsFile &Settings::Display() const
{
  return *m_display;
}

const SettingsFile &Settings::Graphics() const
{
  return *m_graphics;
}

}