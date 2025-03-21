
#include <csCore/resource/csAssetLocator.hh>
#include <csCore/csString.hh>
#include <vector>

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

std::string replace_string(const std::string &str, char search, char replace)
{
  std::string res;
  for (char   ch: str)
  {
    if (ch == search)
    {
      res += replace;
    }
    else
    {
      res += ch;
    }
  }
  return res;
}

std::string replace_string(const std::string &str, const std::string &search, const std::string &replace)
{
  std::string result = str;
  size_t      len    = search.length();
  size_t      idx;

  while ((idx = result.find(search)) != std::string::npos)
  {
    result = result.replace(idx, len, replace);
  }

  return result;
}

std::vector<std::string> split_string(const std::string &str, char delimiter)
{
  std::vector<std::string> result;
  std::string              temp;

  for (char ch: str)
  {
    if (ch == delimiter)
    {
      result.push_back(temp);
      temp.clear();
    }
    else
    {
      temp += ch;
    }
  }

  // Add the last part if any
  result.push_back(temp);

  return result;
}

std::string canonicalize_encoded(const std::string &encoded)
{
  std::string plain = replace_string(encoded, '\\', '/');
  plain = replace_string(plain, "//", "/");
  std::vector<std::string> parts = split_string(plain, '/');

  std::vector<std::string> resultParts;
  for (const std::string &part : parts)
  {
    if (part == ".")
    {
      continue;
    }
    else if (part == "..")
    {
      if (resultParts.empty())
      {
        return encoded;
      }
      else
      {
        resultParts.pop_back();
      }
    }
    else
    {
      resultParts.push_back(part);
    }
  }

  std::string result;
  bool first = true;
  for (const std::string &part : resultParts)
  {
    if (!first)
    {
      result += "/";
    }
    first = false;
    result += part;
  }


  return result;
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

  m_encoded   = m_path + m_filename;
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
