

#include <cache.hh>

#include <iostream>
#include <fstream>
#include <iomanip>

#include <algorithm>

#include <Windows.h>


namespace cs::moc
{


void Cache::Load(const std::string& path)
{
  std::ifstream stream(path + "/.csCache");
  std::string line;
  int l = 1;
  while (std::getline(stream, line))
  {

    size_t idx = line.find(':');
    if (idx == std::string::npos)
    {
      continue;
    }

    std::string sTime0 = line.substr(0, idx);
    line = line.substr(idx + 1);

    idx = line.find(':');
    if (idx == std::string::npos)
    {
      continue;
    }

    std::string sTime1 = line.substr(0, idx);
    line = line.substr(idx + 1);


    idx = line.find(':');
    if (idx == std::string::npos)
    {
      continue;
    }

    std::string sNumClasses = line.substr(0, idx);
    std::string headerName = line.substr(idx + 1);
    int numClasses = std::atoi(sNumClasses.c_str());
    long long time0 = std::atoll(sTime0.c_str());
    long long time1 = std::atoll(sTime1.c_str());

    Data data;
    data.touched = false;
    data.filename = headerName;
    data.cacheTime.dwLowDateTime = (DWORD)time0;
    data.cacheTime.dwHighDateTime = (DWORD)time1;

    getFileTime(headerName, &data.filetime);


    
    for (int i = 0; i < numClasses; ++i)
    {
      if (!std::getline(stream, line))
      {
        break;
      }
      data.classes.push_back(line);
    }

    this->m_fileCache[headerName] = data;

  }

}

void Cache::Store(const std::string& path)
{
  std::ofstream stream(path + "/.csCache");
  for (auto it: m_fileCache)
  {
    Data& data = it.second;
    stream << data.filetime.dwLowDateTime << ":"
      << data.filetime.dwHighDateTime << ":"
      << data.classes.size() << ":"
      << data.filename
      << "\n";

    for (auto cls : data.classes)
    {
      stream << cls << "\n";
    }
  }

}

void Cache::Clear(const std::string& filename)
{
  auto it = m_fileCache.find(filename);
  if (it != m_fileCache.end())
  {
    m_fileCache.erase(it);
  }
}

void Cache::Put(const std::string& filename, const std::string& className)
{
  Data& data = m_fileCache[filename];
  data.touched = true;
  data.filename = filename;
  auto it = std::find(data.classes.begin(), data.classes.end(), className);
  if (it == data.classes.end())
  {
    data.classes.push_back(className);
  }
  if (data.filetime.dwHighDateTime == 0 && data.filetime.dwLowDateTime == 0)
  {

    getFileTime(data.filename, &data.filetime);
  }
}

void Cache::Touch(const std::string& filename)
{
  auto it = m_fileCache.find(filename);
  if (it != m_fileCache.end())
  {
    it->second.touched = true;
    return;
  }

  Data data;
  data.touched = true;
  data.filename = filename;
  data.classes.clear();
  getFileTime(data.filename, &data.filetime);
  m_fileCache[filename] = data;
}

size_t Cache::RemoveAllUntouched()
{
  std::vector<std::string> delete_keys;
  for (auto it = m_fileCache.begin(); it != m_fileCache.end(); it++)
  {
    if (!it->second.touched)
    {
      delete_keys.push_back(it->first);
    }
  }
  for (auto key : delete_keys)
  {
    m_fileCache.erase(key);
  }
  return delete_keys.size();
}

bool Cache::NeedRevalidation(const std::string& filename) const
{
  auto it = m_fileCache.find(filename);
  if (it == m_fileCache.end())
  {
    return true;
  }


  // revalidation is neede when the cached data is older than the real file
  return CompareFileTime(&it->second.cacheTime, &it->second.filetime) < 0;
  //return true;
}

bool Cache::HaveUntouched() const
{
  for (auto entry : m_fileCache)
  {
    if (!entry.second.touched)
    {
      return true;
    }
  }
  return false;
}

const std::map<std::string, Cache::Data>& Cache::GetFileCache() const
{
  return m_fileCache;
}

bool Cache::getFileTime(const std::string& filename, LPFILETIME lpft) const
{
  HANDLE hFile = CreateFile(filename.c_str(), 
    GENERIC_READ, 
    FILE_SHARE_READ, 
    NULL,
    OPEN_EXISTING, 
    0, 
    NULL);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    return false;
  }


  // https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-getfiletime
// https://docs.microsoft.com/de-de/windows/desktop/api/fileapi/nf-fileapi-createfilea
// https://docs.microsoft.com/de-de/windows/desktop/SysInfo/retrieving-the-last-write-time

  return GetFileTime(
    hFile,
    NULL,
    NULL,
    lpft
  );
}


}
