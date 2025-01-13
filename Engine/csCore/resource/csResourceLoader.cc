//
// Created by Marcell on 12.01.2025.
//

#include <csCore/resource/csResourceLoader.hh>

namespace cs
{

csResourceLoader::csResourceLoader(uint32_t priority)
    : iObject()
    , m_priority(priority)
{

}

csResourceLoader::~csResourceLoader ()
{

}


void csResourceLoader::AddExtension(const std::string &extension)
{
  m_extensions.push_back(extension);
}

uint32_t csResourceLoader::GetPriority() const
{
  return m_priority;
}

bool csResourceLoader::IsValidFor(const cs::csAssetLocator &locator) const
{
  for (const auto &item: m_extensions)
  {}
}


} // cs