
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

std::string extract_path(const std::string& locator)
{
  size_t endPos = locator.find_last_of('/');
  if (endPos == std::string::npos)
  {
    return "";
  }

  size_t startPos = 0;
  if (locator._Starts_with("file://"))
  {
    startPos = 7;
  }
  else if (locator._Starts_with("bundle://"))
  {
    startPos = 9;
  }

  if (startPos > endPos)
  {
    return "";
  }

  return locator.substr(startPos, endPos - startPos + 1);
}

std::string extract_filename(const std::string& locator)
{
  size_t pos = locator.find_last_of('/');
  if (pos == std::string::npos)
  {
    pos = 0;
  }
  else
  {
    pos++;
  }
  return locator.substr(pos);
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
  m_path = extract_path(encoded);
  m_filename = extract_filename(encoded);
  m_extension = extract_extension(encoded);

  m_encoded = m_path + m_filename;
}

ResourceLocator::ResourceLocator(const ResourceLocator& parent, const std::string& encoded)
{
  m_proto = extract_proto(encoded);
  m_path = extract_path(encoded);
  m_filename = extract_filename(encoded);
  m_extension = extract_extension(encoded);

  if (m_path.empty() || m_path[0] != '/')
  {
    m_path = parent.m_path + m_path;
  }

  m_encoded = m_path + m_filename;
}


ResourceLocator::ResourceLocator(const ResourceLocator* parent, const std::string& encoded)
{
  m_proto = extract_proto(encoded);
  m_path = extract_path(encoded);
  m_filename = extract_filename(encoded);
  m_extension = extract_extension(encoded);

  if (parent && (m_path.empty() || m_path[0] != '/'))
  {
    m_path = parent->m_path + m_path;
  }

  m_encoded = m_path + m_filename;
}


const std::string& ResourceLocator::GetLocator() const
{
  return m_locator;
}

const std::string& ResourceLocator::GetPath() const
{
  return m_path;
}


const std::string& ResourceLocator::GetFilename() const
{
  return m_filename;
}


const std::string& ResourceLocator::GetExtension() const
{
  return m_extension;
}

const std::string& ResourceLocator::Encoded() const
{
  return m_encoded;
}


bool ResourceLocator::operator<(const ResourceLocator& locator) const
{
  return m_encoded < locator.m_encoded;
}

bool ResourceLocator::operator==(const ResourceLocator& locator) const
{
  return m_encoded == locator.m_encoded;
}

}
