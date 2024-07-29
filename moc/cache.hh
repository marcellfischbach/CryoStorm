
#pragma once

#include <string>
#include <map>
#include <vector>


#include <Windows.h>

namespace ce::moc
{


class Cache
{
public:

  struct Data
  {
    bool touched;
    std::string filename;
    FILETIME filetime;
    FILETIME cacheTime;
    std::vector<std::string> classes;
  };

  void Load(const std::string& path);
  void Store(const std::string& path);

  void Clear(const std::string& filename);
  void Put(const std::string& filename, const std::string& className);
  void Touch(const std::string& filename);

  bool NeedRevalidation(const std::string& filename) const;
  bool HaveUntouched() const;
  size_t RemoveAllUntouched();
  const std::map<std::string, Data>& GetFileCache() const;

private:

  bool getFileTime(const std::string& filename, LPFILETIME lpft) const;

  std::map<std::string, Data> m_fileCache;
};

}
