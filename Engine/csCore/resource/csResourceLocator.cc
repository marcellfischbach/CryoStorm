
#include <csCore/resource/csResourceLocator.hh>
#include <csCore/csString.hh>

namespace cs
{


std::string extract_archive(const std::string& locator)
{
  size_t endPos = locator.find_last_of('@');
  if (endPos == std::string::npos)
  {
    return "";
  }


  return locator.substr(0, endPos);
}


std::string extract_path(const std::string& locator)
{
  size_t endPos = locator.find_last_of('/');
  if (endPos == std::string::npos)
  {
    return "";
  }

  size_t startPos = locator.find_last_of('@');
  if (startPos == std::string::npos)
  {
    startPos = 0;
  }
  else
  {
    startPos++;
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
    return cs::to_upper(locator.substr(idx + 1));
  }
  return "";
}

csResourceLocator::csResourceLocator(const std::string& encoded)
  : m_locator(encoded)
{
  m_archive = extract_archive(encoded);
  m_path = extract_path(encoded);
  m_filename = extract_filename(encoded);
  m_extension = extract_extension(encoded);

  m_encoded = m_path + m_filename;

}

csResourceLocator::csResourceLocator(const csResourceLocator& parent, const std::string& encoded)
{
  m_archive = extract_archive(encoded);
  m_path = extract_path(encoded);
  m_filename = extract_filename(encoded);
  m_extension = extract_extension(encoded);

  if (m_path.empty() || m_path[0] != '/')
  {
    m_path = parent.m_path + m_path;
  }

  m_encoded = m_path + m_filename;
  if (!m_archive.empty())
  {
    m_encoded = m_archive + "@" + m_encoded;
  }
}


csResourceLocator::csResourceLocator(const csResourceLocator* parent, const std::string& encoded)
{
  m_path = extract_path(encoded);
  m_filename = extract_filename(encoded);
  m_extension = extract_extension(encoded);

  if (parent && (m_path.empty() || m_path[0] != '/'))
  {
    m_path = parent->m_path + m_path;
  }

  m_encoded = m_path + m_filename;
}

const std::string& csResourceLocator::GetArchive() const
{
  return m_archive;
}

const std::string& csResourceLocator::GetLocator() const
{
  return m_locator;
}

const std::string& csResourceLocator::GetPath() const
{
  return m_path;
}


const std::string& csResourceLocator::GetFilename() const
{
  return m_filename;
}


const std::string& csResourceLocator::GetExtension() const
{
  return m_extension;
}

const std::string& csResourceLocator::Encoded() const
{
  return m_encoded;
}


bool csResourceLocator::operator<(const csResourceLocator& locator) const
{
  return m_encoded < locator.m_encoded;
}

bool csResourceLocator::operator==(const csResourceLocator& locator) const
{
  return m_encoded == locator.m_encoded;
}

}
