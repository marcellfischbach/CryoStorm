
#pragma once

#include <ceCore/coreexport.hh>
#include <ceCore/class.hh>
#include <ceCore/types.hh>
#include <ceCore/resource/file.hh>
#include <ceCore/resource/resourcelocator.hh>

namespace cryo
{

const int16_t DefaultPriority = 0;

CS_CLASS()
struct CS_CORE_API iAssetLoader : public iObject
{
  CS_CLASS_GEN;
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


}