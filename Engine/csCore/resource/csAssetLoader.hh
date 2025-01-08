#pragma once

#include <csCore/csCoreExport.hh>
#include <csCore/csClass.hh>
#include <csCore/resource/csResourceLocator.hh>
#include <csCore/csRef.hh>
#include <exception>
#include <vector>

namespace cs
{


class DuplicateResourceException : public std::exception
{
public:
  DuplicateResourceException(const std::string& msg)
    : std::exception(msg.c_str())
  {
  }
};



CS_CLASS()
class CS_CORE_API csAssetLoader : public CS_SUPER(iObject)
{
  CS_CLASS_GEN_OBJECT;
protected:
  csAssetLoader();

  void RegisterType(const csClass * cls, const std::string & extension);
  virtual csRef<iObject> Load(const csClass * cls, const csResourceLocator & locator) const = 0;



public:
  virtual ~csAssetLoader();

  struct FileFormat
  {
    const csClass* Cls;
    std::string Extension;
  };



  csRef<iObject> Load(const csResourceLocator & locator) const;
  bool CanLoad(const csResourceLocator & locator) const;

  const std::vector<FileFormat>& GetFormats() const;



private:

  std::vector<FileFormat> m_formats;
};

}
