
#include <csCore/resource/csAssetLoader.hh>

namespace cs
{


csAssetLoader::csAssetLoader()
{

}

csAssetLoader::~csAssetLoader()
{

}

void csAssetLoader::RegisterType(const csClass* cls, const std::string& extension)
{
  for (const auto& fmt : m_formats)
  {
    if (fmt.Extension == extension && fmt.Cls != cls)
    {
      throw DuplicateResourceException("Extension " + extension + " already registered for: " + fmt.Cls->GetName());
    }
  }

  FileFormat fmt{};
  fmt.Cls = cls;
  fmt.Extension = extension;
  m_formats.push_back(fmt);
}

const std::vector<csAssetLoader::FileFormat>& csAssetLoader::GetFormats() const
{
  return m_formats;
}

bool csAssetLoader::CanLoad(const csResourceLocator& locator) const
{
  const std::string &ext = locator.GetExtension();
  for (const auto& fmt : m_formats)
  {
    if (fmt.Extension == ext)
    {
      return true;
    }
  }
  return false;
}


csRef<iObject> csAssetLoader::Load(const csResourceLocator& locator) const
{
  const std::string& ext = locator.GetExtension();
  for (const auto& fmt : m_formats)
  {
    if (fmt.Extension == ext)
    {
      return Load(fmt.Cls, locator);
    }
  }
  return csRef<iObject>::Null();
}

}