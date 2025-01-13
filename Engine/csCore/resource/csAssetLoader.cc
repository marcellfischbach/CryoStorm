
#include <csCore/resource/csAssetLoader.hh>

namespace cs
{


csAssetLoader::csAssetLoader(uint32_t priority)
    : m_priority(priority)
{

}

uint32_t csAssetLoader::GetPriority() const
{
  return m_priority;
}


void csAssetLoader::RegisterType(const std::string &extension)
{
  if (std::find(m_extensions.begin(), m_extensions.end(), extension) != m_extensions.end())
  {
    return;
  }

  m_extensions.push_back(extension);
}

const std::vector<std::string> &csAssetLoader::GetExtensions() const
{
  return m_extensions;
}

bool csAssetLoader::CanLoad(const csAssetLocator &locator) const
{
  const std::string &ext = locator.GetExtension();
  return std::find(m_extensions.begin(), m_extensions.end(), ext) != m_extensions.end();
}


}