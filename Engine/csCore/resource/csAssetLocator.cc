
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/csString.hh>

namespace cs
{


std::string extract_archive(const std::string &locator)
{
  size_t endPos = locator.find_last_of('@');
  if (endPos == std::string::npos)
  {
    return "";
  }


  return locator.substr(0, endPos);
}


std::string extract_path(const std::string &locator)
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

std::string extract_filename(const std::string &locator)
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

std::string extract_extension(const std::string &locator)
{
  size_t idx = locator.rfind(".");
  if (idx != -1)
  {
    return cs::to_upper(locator.substr(idx + 1));
  }
  return "";
}

std::string canonicalize_encoded(const std::string &encoded)
{
  // TODO: Implement path normalization
  return encoded;
}

csAssetLocator::csAssetLocator(const std::string &locator)
    : m_encoded(locator)
{
  m_archive   = extract_archive(locator);
  m_path      = extract_path(locator);
  m_filename  = extract_filename(locator);
  m_extension = extract_extension(locator);

  m_canonical = canonicalize_encoded(m_path + m_filename);
}

csAssetLocator::csAssetLocator(const csAssetLocator &parent, const std::string &locator)
{
  m_archive   = extract_archive(locator);
  m_path      = extract_path(locator);
  m_filename  = extract_filename(locator);
  m_extension = extract_extension(locator);

  if (m_path.empty() || m_path[0] != '/')
  {
    m_path = parent.m_path + m_path;
  }

  m_encoded    = m_path + m_filename;
  if (!m_archive.empty())
  {
    m_encoded = m_archive + "@" + m_encoded;
  }
  m_canonical = canonicalize_encoded(m_path + m_filename);
}

csAssetLocator csAssetLocator::AsAnonymous() const
{
  return csAssetLocator(m_canonical);
}


const std::string &csAssetLocator::GetArchive() const
{
  return m_archive;
}

const std::string &csAssetLocator::GetPath() const
{
  return m_path;
}


const std::string &csAssetLocator::GetFilename() const
{
  return m_filename;
}


const std::string &csAssetLocator::GetExtension() const
{
  return m_extension;
}

const std::string &csAssetLocator::Encoded() const
{
  return m_encoded;
}

const std::string &csAssetLocator::Canonical() const
{
  return m_canonical;
}


bool csAssetLocator::operator<(const csAssetLocator &locator) const
{
  return m_canonical < locator.m_canonical;
}

bool csAssetLocator::operator==(const csAssetLocator &locator) const
{
  return m_canonical == locator.m_canonical;
}

}
