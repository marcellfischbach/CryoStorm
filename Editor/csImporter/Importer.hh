
#pragma once

#include <string>
#include <filesystem>
#include <vector>

namespace std
{
namespace fs = filesystem;
}

namespace cs::imp
{

struct iImporter
{
  virtual ~iImporter() = default;

  virtual bool CanImport (const std::fs::path &path, const std::vector<std::string> &args) const = 0;

  virtual bool Import(const std::fs::path &path, const std::vector<std::string> &args) const = 0;

  virtual void PrintUsage () const = 0;



  bool HasOption(const std::vector<std::string> &options, const std::string &option) const
  {
    for (size_t i=0, in=options.size(); i<in; i++)
    {
      if (options[i] == option)
      {
        return true;
      }
    }
    return false;
  }

  std::string GetOption(const std::vector<std::string> &options, const std::string &option) const
  {
    for (size_t i=0, in=options.size(); i<in; i++)
    {
      if (options[i] == option && (i+1) < in)
      {
        if (options[i+1].substr(0, 2) == std::string ("--"))
        {
          return std::string ();
        }
        return options[i+1];
      }
    }
    return std::string();
  }

};


}