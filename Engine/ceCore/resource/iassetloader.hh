
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/resource/file.hh>
#include <ceCore/resource/resourcelocator.hh>

namespace ce
{

const int16_t DefaultPriority = 0;

CE_CLASS()
struct CE_CORE_API iAssetLoader : public iObject
{
  CE_CLASS_GEN;
  virtual ~iAssetLoader()
  {

  }

  virtual int16_t Priority() const
  {
    return DefaultPriority;
  }

  virtual bool CanLoad(const Class* cls, const ResourceLocator& locator) const = 0;

  virtual iObject* Load(const Class* cls, const ResourceLocator& locator) const = 0;

};



CE_CLASS()
struct CE_CORE_API iAssetLoaderCEF : public iObject
{
  CE_CLASS_GEN;
  virtual ~iAssetLoaderCEF()
  {

  }

  virtual int16_t Priority() const
  {
    return DefaultPriority;
  }

  virtual bool CanLoad(const Class* cls, const file::File* file, const ResourceLocator* locator) const = 0;

  virtual iObject* Load(const Class* cls, const file::File* file, const ResourceLocator* locator) const = 0;

};

}