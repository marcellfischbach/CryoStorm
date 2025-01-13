#include <csCore/csSettings.hh>
#include <csCore/resource/csCryoFile.hh>
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/resource/csVFS.hh>
#

namespace cs
{

csSettingsFile::csSettingsFile(const std::string& locator)
  : m_file(nullptr)
{
  Initialize(csAssetLocator(locator));
}

csSettingsFile::csSettingsFile(const csAssetLocator& locator)
  : m_file(nullptr)
{
  Initialize(locator);
}

void csSettingsFile::Initialize(const csAssetLocator& locator)
{
  iFile* file = csVFS::Get()->Open(locator, eAM_Read, eOM_Binary);
  if (file)
  {
    m_file = new csCryoFile();
    if (!m_file->Parse(file))
    {
      delete m_file;
      m_file = nullptr;
    }
    file->Release();
  }
}

csSettingsFile::~csSettingsFile()
{
  delete m_file;
  m_file = nullptr;
}

bool csSettingsFile::IsValid() const
{
  return m_file;
}

std::string csSettingsFile::GetText(const std::string& path, const std::string& defaultValue) const
{
  csCryoFileElement * element = GetElement(path);
  if (element)
  {
    return element->GetAttribute(0, "");
  }
  return defaultValue;
}

float csSettingsFile::GetFloat(const std::string& path, float defaultValue) const
{
  csCryoFileElement * element = GetElement(path);
  if (element)
  {
    return (float)atof(element->GetAttribute(0, "").c_str());
  }
  return defaultValue;
}

int csSettingsFile::GetInt(const std::string& path, int defaultValue) const
{
  csCryoFileElement * element = GetElement(path);
  if (element)
  {
    return atoi(element->GetAttribute(0, "").c_str());
  }
  return defaultValue;
}

bool csSettingsFile::GetBool(const std::string& path, bool defaultValue) const
{
  std::string v = GetText(path, defaultValue ? "true" : "false");
  return std::string("true") == v
    || std::string("on") == v
    || std::string("yes") == v
    || std::string("positiv") == v
    || std::string("1") == v;
}

csVector2f csSettingsFile::GetVector2f(const std::string& path, const csVector2f& defaultValue) const
{
  csCryoFileElement * element = GetElement(path);
  if (element && element->GetNumberOfAttributes() >= 2)
  {
    return csVector2f(
      (float)element->GetAttribute(0)->GetDoubleValue(),
      (float)element->GetAttribute(1)->GetDoubleValue()
    );
  }
  return defaultValue;
}

csVector3f csSettingsFile::GetVector3f(const std::string& path, const csVector3f& defaultValue) const
{
  csCryoFileElement * element = GetElement(path);
  if (element)
  {
    if (element->GetNumberOfAttributes() >= 3)
    {
      return csVector3f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        (float)element->GetAttribute(2)->GetDoubleValue()
      );
    }
    else if (element->GetNumberOfAttributes() >= 2)
    {
      return csVector3f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        defaultValue.z
      );
    }
    else if (element->GetNumberOfAttributes() >= 1)
    {
      return csVector3f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        defaultValue.y,
        defaultValue.z
      );
    }
  }
  return defaultValue;
}


csVector4f csSettingsFile::GetVector4f(const std::string& path, const csVector4f& defaultValue) const
{
  csCryoFileElement * element = GetElement(path);
  if (element)
  {
    if (element->GetNumberOfAttributes() >= 4)
    {
      return csVector4f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        (float)element->GetAttribute(2)->GetDoubleValue(),
        (float)element->GetAttribute(3)->GetDoubleValue()
      );
    }
    else if (element->GetNumberOfAttributes() >= 3)
    {
      return csVector4f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        (float)element->GetAttribute(2)->GetDoubleValue(),
        defaultValue.w
      );
    }
    else if (element->GetNumberOfAttributes() >= 2)
    {
      return csVector4f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        (float)element->GetAttribute(1)->GetDoubleValue(),
        defaultValue.z,
        defaultValue.w
      );
    }
    else if (element->GetNumberOfAttributes() >= 1)
    {
      return csVector4f(
        (float)element->GetAttribute(0)->GetDoubleValue(),
        defaultValue.y,
        defaultValue.z,
        defaultValue.w
      );
    }

  }
  return defaultValue;
}


csVector2i csSettingsFile::GetVector2i(const std::string& path, const csVector2i& defaultValue) const
{
  csCryoFileElement * element = GetElement(path);
  if (element && element->GetNumberOfAttributes() >= 2)
  {
    return csVector2i(
      element->GetAttribute(0)->GetIntValue(),
      element->GetAttribute(1)->GetIntValue()
    );
  }
  return defaultValue;
}
csCryoFileElement* csSettingsFile::GetElement(const std::string& path) const
{
  return m_file ? m_file->Root()->GetChild(path) : nullptr;
}


csSettings * csSettings::s_settings = nullptr;

csSettings &csSettings::Get()
{
  if (!s_settings)
  {
    s_settings = new csSettings();
  }
  return *s_settings;
}

csSettings::csSettings()
{
  csSettingsFile master ("/settings.config");

  if (master.IsValid())
  {
    m_graphics = new csSettingsFile(master.GetText("graphics"));
    m_display  = new csSettingsFile(master.GetText("display"));
  }
  else
  {
    m_graphics = new csSettingsFile("/${config}/graphics.config");
    m_display = new csSettingsFile("/${config}/display.config");
  }
}

const csSettingsFile &csSettings::Display() const
{
  return *m_display;
}

const csSettingsFile &csSettings::Graphics() const
{
  return *m_graphics;
}

}