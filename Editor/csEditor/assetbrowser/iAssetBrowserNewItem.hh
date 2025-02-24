#pragma once

#include <string>
#include <filesystem>

namespace std
{
namespace fs = filesystem;
}

struct iAssetBrowserNewItem
{

  virtual std::string &GetName() const = 0;

  virtual int GetSortIdx () const
  {
    return 0;
  };

  virtual bool DefaultNamingBehaviour() const
  {
    return true;
  };

  virtual void Create (const std::fs::path &path, const std::string &itemName) = 0;

};