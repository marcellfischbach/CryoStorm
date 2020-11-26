
#include <spcCore/resource/resourcelocator.hh>
#include <spcCore/string.hh>

namespace spc
{


std::string extract_proto(const std::string& locator)
{
  if (locator._Starts_with("file://"))
  {
    return "file://";
  }
  else if (locator._Starts_with("bundle://"))
  {
    return "bundle://";
  }
  else
  {
    return "file://";
  }
}

std::string extract_filename(const std::string& locator)
{
  if (locator._Starts_with("file://"))
  {
    return locator.substr(7);
  }
  else if (locator._Starts_with("bundle://"))
  {
    return locator.substr(9);
  }
  else
  {
    return locator;
  }
}

std::string extract_extension(const std::string& locator)
{
  size_t idx = locator.rfind(".");
  if (idx != -1)
  {
    return spc::to_upper(locator.substr(idx + 1));
  }
  return "";
}

ResourceLocator::ResourceLocator(const std::string& encoded)
  : m_locator(encoded)
{
  m_proto = extract_proto(encoded);
  m_filename = extract_filename(encoded);
  m_extension = extract_extension(encoded);
}


const std::string& ResourceLocator::GetLocator() const
{
  return m_locator;
}


const std::string& ResourceLocator::GetFilename() const
{
  return m_filename;
}


const std::string& ResourceLocator::GetExtension() const
{
  return m_extension;
}

}
